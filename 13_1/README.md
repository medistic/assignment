# 과제 13: 병합 정렬과 퀵 정렬의 성능 비교


## 1. 비교 횟수 측정 기준

Merge Sort는 병합 과정에서 왼쪽 원소와 오른쪽 원소의 값을 비교할 때만 1회로 계산하였다. 반복문의 인덱스 검사, 데이터 이동, 배열 복사는 포함하지 않았다.

Quick Sort는 이전 과제와 동일한 Lomuto partition을 사용하였다. `data[j] <= pivot_value`의 값 비교를 모두 계산하였고, Median-of-Three에서는 첫 번째·가운데·마지막 값 중 중앙값을 찾기 위한 비교도 포함하였다.

## 2. 실행시간 측정 기준

각 알고리즘을 동일 입력으로 10회 반복하고 평균 시간을 사용하였다. 원본 배열 복사는 타이머 시작 전에 수행했으므로 데이터 생성과 배열 복사 시간은 제외하였다. 시간 측정은 C11 `timespec_get()`을 사용했으며 단위는 ms이다.

Random Pivot은 각 반복에서 같은 pivot RNG seed로 초기화하여, 반복마다 동일한 partition 구조가 발생하도록 하고 실행시간만 평균하였다.

## 3. 메모리 사용량 계산 기준

프로세스 전체 메모리(RSS)가 아니라 **정렬 자체가 추가로 요구하는 최대 보조 메모리**를 계산하였다. 원본 데이터 배열은 공통 비용이므로 제외하였다.

Merge Sort는 길이 n의 임시 `int` 배열을 한 번 할당한다. 따라서:

```text
Merge extra memory
= n * sizeof(int)
+ max recursion depth * sizeof(MergeFrameEstimate)
```

으로 계산하였다. 따라서 추가 공간복잡도는 `O(n)`이다.

Quick Sort는 n 크기의 별도 배열을 만들지 않고 재귀 스택만 사용한다. 따라서:

```text
Quick extra memory
= max recursion depth * sizeof(QuickFrameEstimate)
```

로 계산하였다. 평균적으로 균형 잡힌 partition이면 `O(log n)` 보조 공간을 사용하지만, 최악에는 `O(n)`까지 증가할 수 있다.

실제 컴파일러의 물리적 stack frame 크기는 ABI와 최적화 옵션에 따라 달라질 수 있으므로, 여기의 Byte 값은 각 재귀 호출에서 유지되어야 하는 **논리적 상태를 구조체로 모델링한 계산값**이다. 이 기준을 모든 알고리즘에 동일하게 적용하였다.

## 4. 실제 측정 결과

| n | Algorithm | Comparisons | Time (ms) | Extra Memory (Bytes) | Max Depth |
|---:|---|---:|---:|---:|---:|
| 1,000 | Merge Sort | 8,720 | 0.022205 | 4,440 | 11 |
| 1,000 | Quick First | 10,240 | 0.015111 | 880 | 22 |
| 1,000 | Quick Random | 9,829 | 0.021139 | 840 | 21 |
| 1,000 | Quick Median3 | 10,604 | 0.018042 | 640 | 16 |
| 2,000 | Merge Sort | 19,427 | 0.053951 | 8,480 | 12 |
| 2,000 | Quick First | 25,690 | 0.053949 | 960 | 24 |
| 2,000 | Quick Random | 26,254 | 0.051476 | 1,040 | 26 |
| 2,000 | Quick Median3 | 24,439 | 0.059095 | 800 | 20 |
| 4,000 | Merge Sort | 42,821 | 0.183519 | 16,520 | 13 |
| 4,000 | Quick First | 53,621 | 0.138326 | 1,160 | 29 |
| 4,000 | Quick Random | 52,398 | 0.177593 | 1,120 | 28 |
| 4,000 | Quick Median3 | 52,503 | 0.159343 | 800 | 20 |
| 8,000 | Merge Sort | 93,602 | 0.501999 | 32,560 | 14 |
| 8,000 | Quick First | 126,257 | 0.409134 | 1,200 | 30 |
| 8,000 | Quick Random | 114,420 | 0.489224 | 1,120 | 28 |
| 8,000 | Quick Median3 | 111,447 | 0.406652 | 920 | 23 |
| 16,000 | Merge Sort | 203,341 | 1.130979 | 64,600 | 15 |
| 16,000 | Quick First | 269,026 | 0.800617 | 1,400 | 35 |
| 16,000 | Quick Random | 253,106 | 1.039344 | 1,280 | 32 |
| 16,000 | Quick Median3 | 241,623 | 0.906177 | 960 | 24 |
| 20,000 | Merge Sort | 261,063 | 1.415657 | 80,640 | 16 |
| 20,000 | Quick First | 339,525 | 1.069765 | 1,400 | 35 |
| 20,000 | Quick Random | 356,207 | 1.363408 | 1,360 | 34 |
| 20,000 | Quick Median3 | 317,222 | 1.239584 | 1,040 | 26 |

모든 경우에서 `Sorted Verification : Yes`를 확인하였다.

## 5. 데이터 크기 증가에 따른 비교 횟수

Merge Sort는 입력 배열의 값 분포와 관계없이 배열을 계속 절반으로 나눈 뒤 병합하므로 비교 횟수가 비교적 안정적으로 `O(n log n)` 형태로 증가한다.

Quick Sort도 평균적으로는 `O(n log n)`이지만 피벗이 배열을 얼마나 균형 있게 분할하는가에 따라 실제 비교 횟수가 달라진다. 같은 n에서도 First, Random, Median-of-Three가 서로 다른 비교 횟수를 보인 이유가 이것이다.

이번 실험의 각 크기별 결과를 요약하면 다음과 같다.

- n=1,000: 최소 비교 횟수는 **Merge Sort**, 최소 실행시간은 **Quick First**.
- n=2,000: 최소 비교 횟수는 **Merge Sort**, 최소 실행시간은 **Quick Random**.
- n=4,000: 최소 비교 횟수는 **Merge Sort**, 최소 실행시간은 **Quick First**.
- n=8,000: 최소 비교 횟수는 **Merge Sort**, 최소 실행시간은 **Quick Median3**.
- n=16,000: 최소 비교 횟수는 **Merge Sort**, 최소 실행시간은 **Quick First**.
- n=20,000: 최소 비교 횟수는 **Merge Sort**, 최소 실행시간은 **Quick First**.

이번 seed에서는 Merge Sort가 모든 n에서 가장 적은 데이터 값 비교를 사용하였다. Quick Sort는 partition 과정에서 피벗과 많은 원소를 비교하고, 분할이 완전히 균형적이지 않으면 추가 비교가 누적된다.

## 6. 실행시간 차이

비교 횟수가 적다고 해서 실행시간이 반드시 가장 짧은 것은 아니다. Merge Sort는 비교 횟수는 적지만 병합 과정에서 임시 배열에 데이터를 쓰고 다시 원본 배열로 복사해야 한다.

Quick Sort는 배열 내부에서 직접 partition을 수행하기 때문에 추가 배열 복사 비용이 없다. 따라서 일부 n에서는 비교 횟수가 더 많더라도 Quick Sort가 더 짧은 실행시간을 기록하였다.

실제 실행시간은 CPU cache, 메모리 접근 패턴, branch prediction, 운영체제 스케줄링의 영향을 받으므로 실행마다 약간 달라질 수 있다.

## 7. 퀵 정렬 피벗 선택의 영향

**First Pivot**은 구현이 단순하지만 입력 순서에 민감하다. 이미 정렬된 입력에서는 매우 불균형한 partition을 만들 수 있어 최악 `O(n^2)`이 될 수 있다.

**Random Pivot**은 특정 입력 순서가 지속적으로 나쁜 피벗을 만드는 위험을 줄인다. 하지만 매번 중앙에 가까운 값을 뽑는 것은 아니므로 실행마다 분할 품질이 달라질 수 있다.

**Median-of-Three**는 첫 번째·가운데·마지막 값의 중앙값을 사용한다. 피벗을 고르기 위한 추가 비교가 필요하지만 극단값 피벗 가능성을 줄여 재귀 깊이를 안정적으로 낮추는 경향이 있다. 실제 결과에서도 Median-of-Three의 최대 재귀 깊이는 First 또는 Random보다 작은 경우가 많았다.

## 8. 메모리 차이

Merge Sort의 핵심 단점은 n 크기의 임시 배열이 필요하다는 점이다. n이 증가할수록 메모리 사용량도 거의 선형으로 증가하였다.

반면 Quick Sort는 별도의 n 크기 배열 없이 재귀 호출 스택만 사용한다. 이번 무작위 입력에서 재귀 깊이는 n에 비해 매우 작았기 때문에 추가 메모리도 훨씬 작았다.

n=20,000에서:

- Merge Sort: **80,640 Bytes**
- Quick Median3: **1,040 Bytes**

로, Merge Sort가 약 **77.54배** 많은 추가 메모리를 사용하였다.

## 9. n=20,000 직접 비교

비교 횟수:

- Merge Sort: **261,063**
- Quick First: **339,525**
- Quick Random: **356,207**
- Quick Median3: **317,222**

실행시간:

- Merge Sort: **1.415657 ms**
- Quick First: **1.069765 ms**
- Quick Random: **1.363408 ms**
- Quick Median3: **1.239584 ms**

추가 메모리:

- Merge Sort: **80,640 Bytes**
- Quick First: **1,400 Bytes**
- Quick Random: **1,360 Bytes**
- Quick Median3: **1,040 Bytes**

## 10. 시간 및 공간 복잡도

| Algorithm | Average Time | Worst Time | Extra Space |
|---|---|---|---|
| Merge Sort | O(n log n) | O(n log n) | O(n) |
| Quick Sort | O(n log n) | O(n²) | Average O(log n), Worst O(n) |

Merge Sort는 최악 시간까지 안정적인 대신 추가 메모리를 많이 사용한다. Quick Sort는 평균적으로 빠르고 메모리 사용량이 적지만, 피벗 선택이 나쁘면 시간과 재귀 깊이가 모두 커질 수 있다.

## 11. 결론

이번 실험에서는 Merge Sort가 비교 횟수 측면에서 가장 안정적이고 적은 값을 기록하였다. 그러나 Merge Sort는 n 크기의 임시 배열과 병합 과정의 데이터 복사가 필요하여 메모리 사용량이 크고, 실제 실행시간에서는 일부 크기에서 Quick Sort보다 느렸다.

Quick Sort는 메모리 효율이 매우 높았으며, 피벗 선택 방식에 따라 비교 횟수와 최대 재귀 깊이가 달라졌다. 특히 Median-of-Three는 추가 피벗 비교 비용이 있지만 재귀 깊이를 낮추는 경향을 보였다.

따라서 **최악 시간의 안정성과 예측 가능성**이 중요하면 Merge Sort가 유리하고, **낮은 추가 메모리와 평균적인 실제 실행 성능**이 중요하면 Quick Sort가 유리하다고 볼 수 있다.
