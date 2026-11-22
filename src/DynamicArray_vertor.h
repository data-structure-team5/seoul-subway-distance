#pragma once
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

// 프로젝트에서 공통으로 사용할 수 있는 기본 동적 배열 (C++17).
// 재할당이 일어나면 원소를 가리키는 포인터와 참조는 무효가 된다.
template <typename T>
class DynamicArray {
public:
    DynamicArray() = default;

    DynamicArray(const DynamicArray& other) {
        try {
            reserve(other.size_);
            for (const T& value : other) push_back(value);
        } catch (...) {
            release();
            throw;
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            DynamicArray copy(other);
            swap(copy);
        }
        return *this;
    }

    DynamicArray(DynamicArray&& other) noexcept { swap(other); }
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            release();
            swap(other);
        }
        return *this;
    }

    ~DynamicArray() { release(); }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    T& operator[](std::size_t index) { return data_[index]; }
    const T& operator[](std::size_t index) const { return data_[index]; }
    T* begin() { return data_; }
    const T* begin() const { return data_; }
    T* end() { return data_ ? data_ + size_ : nullptr; }
    const T* end() const { return data_ ? data_ + size_ : nullptr; }

    void clear() noexcept {
        while (size_ > 0) {
            --size_;
            Traits::destroy(allocator_, data_ + size_);
        }
    }

    void reserve(std::size_t requested) {
        if (requested <= capacity_) return;
        if (requested > Traits::max_size(allocator_)) throw std::length_error("DynamicArray capacity exceeded");
        T* newData = Traits::allocate(allocator_, requested);
        std::size_t constructed = 0;
        try {
            for (; constructed < size_; ++constructed)
                Traits::construct(allocator_, newData + constructed, data_[constructed]);
        } catch (...) {
            while (constructed > 0) Traits::destroy(allocator_, newData + --constructed);
            Traits::deallocate(allocator_, newData, requested);
            throw;
        }
        const std::size_t oldSize = size_;
        clear();
        if (data_) Traits::deallocate(allocator_, data_, capacity_);
        data_ = newData;
        size_ = oldSize;
        capacity_ = requested;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            // value가 이 배열의 원소여도 재할당 전에 안전하게 복사한다.
            T copy(value);
            const std::size_t max = Traits::max_size(allocator_);
            if (capacity_ == max) throw std::length_error("DynamicArray capacity exceeded");
            const std::size_t next = capacity_ == 0 ? 1 : (capacity_ > max / 2 ? max : capacity_ * 2);
            reserve(next);
            Traits::construct(allocator_, data_ + size_, copy);
        } else {
            Traits::construct(allocator_, data_ + size_, value);
        }
        ++size_;
    }

private:
    using Traits = std::allocator_traits<std::allocator<T>>;
    std::allocator<T> allocator_;
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;

    void swap(DynamicArray& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

    void release() noexcept {
        clear();
        if (data_) Traits::deallocate(allocator_, data_, capacity_);
        data_ = nullptr;
        capacity_ = 0;
    }
};
