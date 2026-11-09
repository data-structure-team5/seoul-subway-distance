# DynamicArray 사용 안내

`src/DynamicArray_vertor.h`는 `<vector>`를 사용하지 않고 구현한 공통 동적 배열입니다. 원소를 연속된 공간에 저장하고, 공간이 부족하면 더 큰 공간을 확보해 기존 원소를 복사합니다. 여러 자료형에 사용할 수 있도록 `DynamicArray<T>` 템플릿으로 작성했습니다. 현재 코드는 **C++17**을 기준으로 합니다.

## 사용 예시

```cpp
#include "DynamicArray_vertor.h"
#include <iostream>

int main() {
    DynamicArray<int> vertices;
    vertices.push_back(10);
    vertices.push_back(25);

    std::cout << vertices.size() << '\n'; // 2
    std::cout << vertices[0] << '\n';    // 10

    for (int vertex : vertices) {
        std::cout << vertex << ' ';
    }
}
```

다른 폴더에서 헤더를 포함할 때는 빌드 설정의 include 경로 또는 상대 경로에 맞춰 주세요. 문자열은 `#include <string>`을 추가하고 `DynamicArray<std::string>`으로 저장할 수 있습니다.

## 제공하는 기능

| 기능 | 설명 |
|---|---|
| `push_back(value)` | 맨 뒤에 원소를 복사해 추가합니다. |
| `array[index]` | 인덱스로 원소를 읽거나 수정합니다. 범위 검사는 하지 않습니다. |
| `size()` | 현재 저장된 원소 수를 반환합니다. |
| `capacity()` | 재할당 없이 담을 수 있는 원소 수를 반환합니다. |
| `empty()` | 원소가 없으면 `true`를 반환합니다. |
| `reserve(n)` | 원소 수는 유지하면서 최소 `n`개의 공간을 확보합니다. |
| `clear()` | 모든 원소를 제거합니다. 확보한 공간은 유지합니다. |
| `begin()`, `end()` | 처음과 마지막 다음 위치를 반환하며, 범위 기반 `for`문에서 사용할 수 있습니다. |
| 복사·이동 | 복사는 별도의 공간에 원소를 복사하고, 이동은 저장 공간의 소유권을 넘깁니다. |

용량이 부족하면 `push_back()`은 처음에는 용량을 1로 만들고, 이후에는 보통 두 배로 늘립니다. 원소 접근은 `O(1)`, 추가는 **분할 상환 `O(1)`**이며, 재할당이 일어나는 한 번의 추가는 기존 원소를 복사하므로 `O(n)`입니다. 원소 자체의 복사 비용은 별도로 고려해야 합니다.

## 주의 사항 및 현재 범위

- 인덱스는 `0`부터 `size() - 1`까지 유효합니다. `operator[]`가 범위를 검사하지 않으므로 호출하는 쪽에서 확인해야 합니다.
- `push_back()`이나 `reserve()`가 재할당하면 **기존 원소를 가리키던 포인터, 참조, 반복 위치가 무효**가 됩니다. `clear()` 후에도 제거된 원소의 포인터나 참조를 사용하면 안 됩니다.
- 현재 추가와 배열 복사·확장은 원소의 복사 생성을 사용하므로, 저장할 자료형은 해당 작업에서 복사 가능해야 합니다.
- `std::vector`의 전체 대체품은 아닙니다. `pop_back()`, `resize()`, `at()`, 중간 삽입·삭제 등은 현재 구현되어 있지 않습니다.
- 이 헤더는 `<vector>`를 사용하지 않습니다. `<memory>`의 `std::allocator`로 저장 공간을 관리하며, `<cstddef>`, `<stdexcept>`, `<utility>`도 사용합니다. 과제에서 허용하는 표준 헤더의 범위는 별도로 확인이 필요합니다.

**통합 상태:** 현재 `Meeting.cpp`는 검색 결과를 `std::vector<int>`로 받고 있어 `DynamicArray<int>`와 형식이 맞지 않습니다. `Meeting.cpp`는 수정하지 않았으며, 팀원과 반환·사용 형식을 맞추기 전까지 전체 프로젝트 컴파일이 되지 않습니다. 다른 모듈에도 `<vector>` 사용이 남아 있습니다.
