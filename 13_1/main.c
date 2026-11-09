#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VALUE_MAX 1000000
#define SIZE_COUNT 6
#define TIME_REPEATS 10

typedef long long Count;

typedef enum { PIVOT_FIRST=0, PIVOT_RANDOM, PIVOT_MEDIAN3 } PivotMethod;

typedef struct { uint64_t state; } RNG;
typedef struct { Count comparisons; int max_depth; size_t memory_bytes; } Stats;
typedef struct { Stats stats; double ms; } Result;

typedef struct { int left,right,middle; int *data,*temp; Count *cmp; } MergeFrameEstimate;
typedef struct { int low,high,pivot; int *data; RNG *rng; Count *cmp; } QuickFrameEstimate;

static uint64_t splitmix64_next(RNG *r){
    uint64_t z=(r->state += 0x9E3779B97F4A7C15ULL);
    z=(z^(z>>30))*0xBF58476D1CE4E5B9ULL;
    z=(z^(z>>27))*0x94D049BB133111EBULL;
    return z^(z>>31);
}

static uint64_t xorshift_next(RNG *r){
    uint64_t x=r->state ? r->state : 0x9E3779B97F4A7C15ULL;
    x^=x>>12; x^=x<<25; x^=x>>27; r->state=x;
    return x*2685821657736338717ULL;
}

static int random_value(RNG *r){ return (int)(splitmix64_next(r) % (VALUE_MAX+1ULL)); }
static void copy_data(int *dst,const int *src,int n){ memcpy(dst,src,sizeof(int)*(size_t)n); }
static void swap_int(int *a,int *b){ int t=*a; *a=*b; *b=t; }
static int is_sorted(const int *a,int n){ for(int i=1;i<n;i++) if(a[i-1]>a[i]) return 0; return 1; }

static double elapsed_ms(const struct timespec *s,const struct timespec *e){
    return (double)(e->tv_sec-s->tv_sec)*1000.0 + (double)(e->tv_nsec-s->tv_nsec)/1000000.0;
}

/* ---------------- Merge Sort ---------------- */
static void merge_ranges(int *a,int *tmp,int l,int m,int r,Count *cmp){
    int i=l,j=m+1,k=l;
    while(i<=m && j<=r){
        (*cmp)++;
        if(a[i]<=a[j]) tmp[k++]=a[i++]; else tmp[k++]=a[j++];
    }
    while(i<=m) tmp[k++]=a[i++];
    while(j<=r) tmp[k++]=a[j++];
    for(i=l;i<=r;i++) a[i]=tmp[i];
}

static void merge_rec(int *a,int *tmp,int l,int r,Count *cmp,int depth,int *max_depth){
    if(depth>*max_depth) *max_depth=depth;
    if(l>=r) return;
    int m=l+(r-l)/2;
    merge_rec(a,tmp,l,m,cmp,depth+1,max_depth);
    merge_rec(a,tmp,m+1,r,cmp,depth+1,max_depth);
    merge_ranges(a,tmp,l,m,r,cmp);
}

static Stats merge_sort(int *a,int n){
    Stats s={0,0,0};
    int *tmp=(int*)malloc(sizeof(int)*(size_t)n);
    if(!tmp){ s.comparisons=-1; return s; }
    merge_rec(a,tmp,0,n-1,&s.comparisons,1,&s.max_depth);
    s.memory_bytes=sizeof(int)*(size_t)n + sizeof(MergeFrameEstimate)*(size_t)s.max_depth;
    free(tmp);
    return s;
}

/* ---------------- Quick Sort ---------------- */
static int median3_index(const int *a,int l,int h,Count *cmp){
    int m=l+(h-l)/2, x=a[l],y=a[m],z=a[h];
    (*cmp)++;
    if(x<y){
        (*cmp)++; if(y<z) return m;
        (*cmp)++; if(x<z) return h;
        return l;
    } else {
        (*cmp)++; if(x<z) return l;
        (*cmp)++; if(y<z) return h;
        return m;
    }
}

static int choose_pivot(const int *a,int l,int h,PivotMethod method,RNG *rng,Count *cmp){
    if(method==PIVOT_FIRST) return l;
    if(method==PIVOT_RANDOM) return l+(int)(xorshift_next(rng)%(uint64_t)(h-l+1));
    return median3_index(a,l,h,cmp);
}

static int partition_lomuto(int *a,int l,int h,PivotMethod method,RNG *rng,Count *cmp){
    int p=choose_pivot(a,l,h,method,rng,cmp);
    swap_int(&a[p],&a[h]);
    int pv=a[h],i=l-1;
    for(int j=l;j<h;j++){
        (*cmp)++;
        if(a[j]<=pv){ i++; if(i!=j) swap_int(&a[i],&a[j]); }
    }
    swap_int(&a[i+1],&a[h]);
    return i+1;
}

static void quick_rec(int *a,int l,int h,PivotMethod method,RNG *rng,Count *cmp,int depth,int *max_depth){
    if(depth>*max_depth) *max_depth=depth;
    if(l>=h) return;
    int p=partition_lomuto(a,l,h,method,rng,cmp);
    quick_rec(a,l,p-1,method,rng,cmp,depth+1,max_depth);
    quick_rec(a,p+1,h,method,rng,cmp,depth+1,max_depth);
}

static Stats quick_sort(int *a,int n,PivotMethod method,RNG *rng){
    Stats s={0,0,0};
    quick_rec(a,0,n-1,method,rng,&s.comparisons,1,&s.max_depth);
    s.memory_bytes=sizeof(QuickFrameEstimate)*(size_t)s.max_depth;
    return s;
}

static uint64_t pivot_seed(uint64_t seed,int n,PivotMethod m){
    return seed ^ ((uint64_t)n<<32) ^ ((uint64_t)m*0x9E3779B97F4A7C15ULL);
}

static int measure_merge(const int *orig,int n,Result *out){
    int *work=(int*)malloc(sizeof(int)*(size_t)n); if(!work) return 0;
    double total=0.0;
    for(int r=0;r<TIME_REPEATS;r++){
        struct timespec s,e; copy_data(work,orig,n);
        timespec_get(&s,TIME_UTC); Stats st=merge_sort(work,n); timespec_get(&e,TIME_UTC);
        if(st.comparisons<0 || !is_sorted(work,n)){ free(work); return 0; }
        total+=elapsed_ms(&s,&e);
        if(r==0) out->stats=st;
        else if(st.comparisons!=out->stats.comparisons || st.memory_bytes!=out->stats.memory_bytes){ free(work); return 0; }
    }
    out->ms=total/TIME_REPEATS; free(work); return 1;
}

static int measure_quick(const int *orig,int n,uint64_t seed,PivotMethod method,Result *out){
    int *work=(int*)malloc(sizeof(int)*(size_t)n); if(!work) return 0;
    double total=0.0;
    for(int r=0;r<TIME_REPEATS;r++){
        RNG rng={pivot_seed(seed,n,method)}; struct timespec s,e; copy_data(work,orig,n);
        timespec_get(&s,TIME_UTC); Stats st=quick_sort(work,n,method,&rng); timespec_get(&e,TIME_UTC);
        if(!is_sorted(work,n)){ free(work); return 0; }
        total+=elapsed_ms(&s,&e);
        if(r==0) out->stats=st;
        else if(st.comparisons!=out->stats.comparisons || st.memory_bytes!=out->stats.memory_bytes){ free(work); return 0; }
    }
    out->ms=total/TIME_REPEATS; free(work); return 1;
}

static void print_row(const char *name,const Result *r){
    printf("%-18s %15lld %12.6f %15zu %10d\n",name,r->stats.comparisons,r->ms,r->stats.memory_bytes,r->stats.max_depth);
}

int main(int argc,char **argv){
    const int sizes[SIZE_COUNT]={1000,2000,4000,8000,16000,20000};
    uint64_t seed=(argc>=2)?strtoull(argv[1],NULL,10):(uint64_t)time(NULL);
    RNG data_rng={seed};
    printf("Random seed : %llu\n",(unsigned long long)seed);
    printf("Value range : 0 ~ %d\n",VALUE_MAX);
    printf("Timing      : average of %d runs\n\n",TIME_REPEATS);

    for(int si=0;si<SIZE_COUNT;si++){
        int n=sizes[si];
        int *orig=(int*)malloc(sizeof(int)*(size_t)n); if(!orig){ fprintf(stderr,"memory error\n"); return 1; }
        for(int i=0;i<n;i++) orig[i]=random_value(&data_rng);

        Result merge,first,random,median;
        if(!measure_merge(orig,n,&merge) || !measure_quick(orig,n,seed,PIVOT_FIRST,&first) ||
           !measure_quick(orig,n,seed,PIVOT_RANDOM,&random) || !measure_quick(orig,n,seed,PIVOT_MEDIAN3,&median)){
            fprintf(stderr,"measurement failed for n=%d\n",n); free(orig); return 1;
        }

        printf("Data Size : %d\n",n);
        printf("%-18s %15s %12s %15s %10s\n","Algorithm","Comparisons","Time(ms)","Memory(Bytes)","MaxDepth");
        printf("--------------------------------------------------------------------------\n");
        print_row("Merge Sort",&merge);
        print_row("Quick (First)",&first);
        print_row("Quick (Random)",&random);
        print_row("Quick (Median3)",&median);
        printf("Sorted Verification : Yes\n\n");
        free(orig);
    }
    return 0;
}
