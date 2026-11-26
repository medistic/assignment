#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <errno.h>

#define NAME_CAP 256
#define LINE_CAP 4096
#define MAX_K 1000
#define SEED UINT64_C(20261126)

typedef struct {
    char id[32], name[NAME_CAP], gender[32];
    int korean, english, math, total;
    size_t original_index;
} Student;
typedef uint64_t Count;

/* CSV: comma-separated fields, optional quotes and escaped double quotes.
   Each student occupies a single physical line. */
static int split_csv(char *line, char *field[6]) {
    char *r=line, *w=line;
    for (int i=0; i<6; ++i) {
        field[i]=w;
        if (*r=='"') {
            ++r;
            for (;;) {
                if (!*r || *r=='\n' || *r=='\r') return 0;
                if (*r=='"') {
                    ++r;
                    if (*r=='"') { *w++='"'; ++r; }
                    else break;
                } else *w++=*r++;
            }
        } else {
            while (*r && *r!=',' && *r!='\n' && *r!='\r') *w++=*r++;
        }
        char delim=*r;
        if (i<5) { if (delim!=',') return 0; ++r; }
        else if (delim!=0 && delim!='\r' && delim!='\n') return 0;
        *w++='\0';
    }
    return 1;
}
static int score(const char *s, int *out) {
    char *end; errno=0;
    long v=strtol(s,&end,10);
    if (errno || end==s || *end || v<0 || v>100) return 0;
    *out=(int)v; return 1;
}
static Student *load_csv(const char *path, size_t *n) {
    FILE *f=fopen(path,"rb");
    if (!f) { perror(path); return NULL; }
    char line[LINE_CAP]; char *c[6];
    Student *a=NULL; size_t cap=0, row=1; *n=0;
    if (!fgets(line,sizeof line,f)) goto bad;
    char *header=line;
    if (strlen(line)>=3 && (unsigned char)line[0]==0xef && (unsigned char)line[1]==0xbb && (unsigned char)line[2]==0xbf) header+=3;
    if (!split_csv(header,c)) goto bad;
    const char *expected[]={"ID","NAME","GENDER","KOREAN_GRADE","ENGLISH_GRADE","MATH_GRADE"};
    for (int i=0;i<6;++i) if (strcmp(c[i],expected[i])) goto bad;
    while (fgets(line,sizeof line,f)) {
        ++row;
        if (!strchr(line,'\n') && !feof(f)) goto bad;
        if (!split_csv(line,c)) goto bad;
        if (!*c[0] || !*c[1] || !*c[2] || strlen(c[0])>=sizeof a[0].id || strlen(c[1])>=sizeof a[0].name || strlen(c[2])>=sizeof a[0].gender) goto bad;
        if (*n==cap) {
            size_t next=cap?cap*2:1024;
            if (next<cap || next>SIZE_MAX/sizeof *a) goto bad;
            Student *p=realloc(a,next*sizeof *a);
            if (!p) goto bad;
            a=p; cap=next;
        }
        Student *s=&a[*n];
        if (!score(c[3],&s->korean)||!score(c[4],&s->english)||!score(c[5],&s->math)) goto bad;
        strcpy(s->id,c[0]); strcpy(s->name,c[1]); strcpy(s->gender,c[2]);
        s->total=s->korean+s->english+s->math;
        s->original_index=*n;
        ++*n;
    }
    if (ferror(f)||!*n) goto bad;
    fclose(f); return a;
bad:
    fprintf(stderr,"CSV error at line %zu (expected ID,NAME,GENDER,KOREAN_GRADE,ENGLISH_GRADE,MATH_GRADE).\n",row);
    fclose(f); free(a); return NULL;
}

/* Exactly one counted comparison per strcmp call used in an algorithm. */
static int name_cmp(const char *a,const char *b,Count *count) {
    ++*count; return strcmp(a,b);
}
static void swap_student(Student *a,Student *b) {
    if (a!=b) { Student t=*a; *a=*b; *b=t; }
}
/* Median-of-three selection; all value comparisons counted. */
static const char *median_name(const Student *a,size_t lo,size_t hi,Count *count) {
    const char *x=a[lo].name,*y=a[lo+(hi-lo)/2].name,*z=a[hi-1].name;
    if (name_cmp(x,y,count)<0) {
        if (name_cmp(y,z,count)<0) return y;
        return name_cmp(x,z,count)<0?z:x;
    }
    if (name_cmp(x,z,count)<0) return x;
    return name_cmp(y,z,count)<0?z:y;
}
/* Three-way partition [lo,lt) < pivot, [lt,gt) == pivot, [gt,hi) > pivot.
   Recurse on the smaller side to bound stack depth. */
static void quick_sort(Student *a,size_t lo,size_t hi,Count *count) {
    while (hi-lo>1) {
        char pivot[NAME_CAP]; strcpy(pivot,median_name(a,lo,hi,count));
        size_t lt=lo,i=lo,gt=hi;
        while (i<gt) {
            int c=name_cmp(a[i].name,pivot,count);
            if (c<0) swap_student(&a[lt++],&a[i++]);
            else if (c>0) swap_student(&a[i],&a[--gt]);
            else ++i;
        }
        if (lt-lo<hi-gt) { quick_sort(a,lo,lt,count); lo=gt; }
        else { quick_sort(a,gt,hi,count); hi=lt; }
    }
}
static size_t sequential_search(const Student *a,size_t n,const char *key,Count *count,size_t *hits) {
    size_t found=0;
    for (size_t i=0;i<n;++i)
        if (name_cmp(a[i].name,key,count)==0) hits[found++]=a[i].original_index;
    return found;
}
static size_t binary_search_all(const Student *a,size_t n,const char *key,Count *count,size_t *hits) {
    size_t lo=0,hi=n,found=0;
    /* lower_bound: first name not less than key */
    while (lo<hi) {
        size_t m=lo+(hi-lo)/2;
        if (name_cmp(a[m].name,key,count)<0) lo=m+1; else hi=m;
    }
    /* Count every equality check including the first nonmatching name. */
    while (lo<n && name_cmp(a[lo].name,key,count)==0)
        hits[found++]=a[lo++].original_index;
    return found;
}
static int same_hits(const size_t *a,size_t na,const size_t *b,size_t nb,unsigned char *seen,size_t n) {
    if (na!=nb) return 0;
    for (size_t i=0;i<na;++i) { if (a[i]>=n || seen[a[i]]) return 0; seen[a[i]]=1; }
    for (size_t i=0;i<nb;++i) { if (b[i]>=n || !seen[b[i]]) return 0; seen[b[i]]=0; }
    return 1;
}
static int verify_sort(const Student *src,const Student *a,size_t n,unsigned char *seen) {
    for (size_t i=0;i<n;++i) {
        size_t j=a[i].original_index;
        if (j>=n || seen[j] || (i && strcmp(a[i-1].name,a[i].name)>0)) return 0;
        const Student *s=&src[j];
        if (strcmp(a[i].id,s->id)||strcmp(a[i].name,s->name)||strcmp(a[i].gender,s->gender)||a[i].korean!=s->korean||a[i].english!=s->english||a[i].math!=s->math||a[i].total!=s->total) return 0;
        seen[j]=1;
    }
    memset(seen,0,n); return 1;
}
static uint64_t rng_next(uint64_t *state) {
    uint64_t z=(*state+=UINT64_C(0x9e3779b97f4a7c15));
    z=(z^(z>>30))*UINT64_C(0xbf58476d1ce4e5b9);
    z=(z^(z>>27))*UINT64_C(0x94d049bb133111eb);
    return z^(z>>31);
}
static int checkpoint(size_t k) { return k<=30||k==50||k==100||k==200||k==500||k==1000; }
static void print_student(const Student *s) {
    printf("ID      : %s\nName    : %s\nGender  : %s\nKorean  : %d\nEnglish : %d\nMath    : %d\nTotal   : %d\n\n",s->id,s->name,s->gender,s->korean,s->english,s->math,s->total);
}
int main(int argc,char **argv) {
    const char *path=argc>1?argv[1]:"students.csv";
    size_t n=0; Student *original=load_csv(path,&n);
    if (!original) return 1;
    Student *sorted=malloc(n*sizeof *sorted);
    size_t *sh=calloc(n,sizeof *sh),*bh=calloc(n,sizeof *bh);
    unsigned char *seen=calloc(n,1);
    int status=1;
    if (!sorted||!sh||!bh||!seen) { fprintf(stderr,"Allocation failed\n"); goto done; }
    memcpy(sorted,original,n*sizeof *sorted);
    Count sort_count=0; quick_sort(sorted,0,n,&sort_count);
    if (!verify_sort(original,sorted,n,seen)) { fprintf(stderr,"Sort verification failed\n"); goto done; }
    size_t distinct=1,best_start=0,best_len=1,start=0;
    for (size_t i=1;i<=n;++i) {
        if (i==n || strcmp(sorted[i-1].name,sorted[i].name)!=0) {
            if (i-start>best_len) { best_start=start; best_len=i-start; }
            if (i<n) ++distinct;
            start=i;
        }
    }
    /* Demo name supplied by user, otherwise most frequent name. */
    const char *key=argc>2?argv[2]:sorted[best_start].name;
    Count sc=0,bc=0;
    size_t a=sequential_search(original,n,key,&sc,sh),b=binary_search_all(sorted,n,key,&bc,bh);
    if (!same_hits(sh,a,bh,b,seen,n)) { fprintf(stderr,"Search verification failed\n"); goto done; }
    printf("Students: %zu\nDistinct names: %zu\nQuick Sort comparisons: %" PRIu64 "\nSorted verification: Yes\n\nSearch Result: %s\nMatches: %zu\n",n,distinct,sort_count,key,a);
    if (!a) puts("Not found");
    for (size_t i=0;i<a;++i) print_student(&original[sh[i]]);
    printf("Demo sequential comparisons: %" PRIu64 "\nDemo binary comparisons: %" PRIu64 "\nDemo result match: Yes\n\n",sc,bc);
    /* A guaranteed missing name; finding it is test preparation, not measured. */
    char absent[NAME_CAP]="__NOT_FOUND__";
    for (unsigned j=0;;++j) {
        int exists=0;
        snprintf(absent,sizeof absent,"__NOT_FOUND_%u__",j);
        for (size_t i=0;i<n;++i) if (strcmp(original[i].name,absent)==0) {exists=1;break;}
        if (!exists) break;
    }
    uint64_t state=SEED;
    Count seq_total=0,bin_total=0;
    size_t crossover=0,existing=0,missing=0;
    printf("Comparison Results (seed=%" PRIu64 ")\n",SEED);
    printf("%6s %16s %16s %16s\n","K","Sequential","Quick + Binary","Binary only");
    for (size_t k=1;k<=MAX_K;++k) {
        const char *q;
        if (k%5==0) {q=absent;++missing;}
        else {q=original[rng_next(&state)%n].name;++existing;}
        a=sequential_search(original,n,q,&seq_total,sh);
        b=binary_search_all(sorted,n,q,&bin_total,bh);
        if (!same_hits(sh,a,bh,b,seen,n)) { fprintf(stderr,"Results differ at K=%zu\n",k);goto done; }
        if (!crossover && sort_count+bin_total<seq_total) crossover=k;
        if (checkpoint(k)) printf("%6zu %16" PRIu64 " %16" PRIu64 " %16" PRIu64 "\n",k,seq_total,sort_count+bin_total,bin_total);
    }
    printf("Existing-name queries: %zu\nMissing-name queries: %zu\n",existing,missing);
    if (crossover) printf("First K where Quick + Binary wins: %zu\n",crossover);
    else puts("No crossover within tested K");
    puts("All search result sets match: Yes"); status=0;
done:
    free(original);free(sorted);free(sh);free(bh);free(seen);return status;
}
