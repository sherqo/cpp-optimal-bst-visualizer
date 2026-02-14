/**
 * This class represents a dynamic array (vector) that can grow in size as needed.
 * It supports basic operations such as resizing, accessing elements, and displaying the contents.
 */

#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class Vector
{
  T *data;    // Pointer to the array of elements
  size_t cap; // Capacity of the vector (maximum number of elements it can hold)
  size_t len; // Current size of the vector (number of elements currently stored)

public:
  /**a
   * @brief Constructor to initialize  vector with a specified size.
   *
   * If an initial size is provided, the vector is created with that many elements, each default-initialized.
   * If no size is provided, an empty vector is created.
   *
   * @param initial_size The initial size of the vector (default is 0).
   */
  Vector(size_t initial_size = 0)
      : data(initial_size ? new T[initial_size] : nullptr), cap(initial_size), len(initial_size) {}

  /**
   * @brief Constructor to initialize a vector with a list of values.
   *
   * This constructor creates a vector with the specified elements from an initializer list.
   *
   * @param list An initializer list of elements.
   */
  Vector(std::initializer_list<T> list)
      : data(new T[list.size()]), cap(list.size()), len(list.size())
  {
    size_t i = 0;
    for (const T &elem : list)
    {
      data[i++] = elem;
    }
  }

  /**
   * @brief Destructor to clean up memory allocated for the vector.
   *
   * The destructor frees the dynamically allocated memory for the vector's elements.
   */
  ~Vector()
  {
    delete[] data;
  }

  /**
   * @brief Copy constructor.
   *
   * Creates a deep copy of another vector.
   *
   * @param other The vector to copy from.
   */
  Vector(const Vector &other)
      : data(other.len ? new T[other.len] : nullptr), cap(other.cap), len(other.len)
  {
    for (size_t i = 0; i < len; ++i)
    {
      data[i] = other.data[i];
    }
  }

  /**
   * @brief Copy assignment operator.
   *
   * Assigns a deep copy of another vector to this vector.
   *
   * @param other The vector to copy from.
   * @return A reference to this vector.
   */
  Vector &operator=(const Vector &other)
  {
    if (this != &other)
    {
      // Clean up existing data
      delete[] data;

      // Copy new data
      cap = other.cap;
      len = other.len;
      data = other.len ? new T[other.len] : nullptr;

      for (size_t i = 0; i < len; ++i)
      {
        data[i] = other.data[i];
      }
    }
    return *this;
  }

  /**
   * @brief Move constructor.
   *
   * Transfers ownership of resources from another vector.
   *
   * @param other The vector to move from.
   */
  Vector(Vector &&other) noexcept
      : data(other.data), cap(other.cap), len(other.len)
  {
    other.data = nullptr;
    other.cap = 0;
    other.len = 0;
  }

  /**
   * @brief Move assignment operator.
   *
   * Transfers ownership of resources from another vector.
   *
   * @param other The vector to move from.
   * @return A reference to this vector.
   */
  Vector &operator=(Vector &&other) noexcept
  {
    if (this != &other)
    {
      // Clean up existing data
      delete[] data;

      // Transfer ownership
      data = other.data;
      cap = other.cap;
      len = other.len;

      // Reset other
      other.data = nullptr;
      other.cap = 0;
      other.len = 0;
    }
    return *this;
  }

  /**
   * @brief Access element at the specified index.
   *
   * This function returns a reference to the element at the specified index. If the index is out of bounds,
   * an exception is thrown.
   *
   * @param index The index of the element to access.
   * @return A reference to the element at the specified index.
   * @throws std::out_of_range If the index is out of bounds.
   */
  T &operator[](size_t index) const
  {
    if (index >= len)
    {
      throw std::out_of_range("Index out of bounds in Vector::operator[]");
    }
    return data[index];
  }

  /**
   * @brief Resize the vector to a new size.
   *
   * This function resizes the vector to the specified size. If the new size is greater than the current capacity,
   * the capacity is increased. New elements are default-initialized if the new size is larger than the current size.
   *
   * @param new_size The new size for the vector.
   */
  void resize(size_t new_size)
  {
    if (new_size > cap)
    {
      // Allocate new memory with some growth factor
      size_t new_cap = (new_size > cap * 2) ? new_size : cap * 2;
      if (new_cap == 0)
        new_cap = 1;

      T *new_data = new T[new_cap];
      for (size_t i = 0; i < len; ++i)
      {
        new_data[i] = data[i]; // Copy existing elements
      }
      delete[] data;
      data = new_data;
      cap = new_cap;
    }
    // Default-initialize new elements if resizing to a larger size
    for (size_t i = len; i < new_size; ++i)
    {
      data[i] = T();
    }
    len = new_size;
  }

  /**
   * @brief Get the current size of the vector.
   *
   * This function returns the current size of the vector (the number of elements stored in the vector).
   *
   * @return The number of elements currently stored in the vector.
   */
  size_t size() const
  {
    return len;
  }

  /**
   * @brief Display the elements of the vector.
   *
   * This function prints all the elements of the vector to the console.
   */
  void display(bool printFirstElement = true, std::string sep = " ") const
  {
    if (len == 0)
    {
      std::cout << std::endl;
      return;
    }

    size_t start = printFirstElement ? 0 : 1;
    
    if (start < len)
    {
      std::cout << data[start];
      for (size_t i = start + 1; i < len; ++i)
        std::cout << sep << data[i];
    }

    std::cout << std::endl;
  }

  /**
   * @brief Add a new element to the end of the vector.
   *
   * If the current capacity is not sufficient, the vector's capacity is doubled before adding the new element.
   *
   * @param value The value to be added to the vector.
   */
  void push_back(const T &value)
  {
    if (len >= cap)
    {
      // Need to grow capacity
      size_t new_cap = (cap == 0) ? 1 : cap * 2;
      T *new_data = new T[new_cap];
      for (size_t i = 0; i < len; ++i)
      {
        new_data[i] = data[i];
      }
      delete[] data;
      data = new_data;
      cap = new_cap;
    }
    data[len++] = value;
  }

  /**
   * @brief Find the first occurrence of a value.
   *
   * @param key The value to search for.
   * @return The index of the first occurrence, or -1 if not found.
   */
  int findOne(const T &key) const
  {
    for (size_t i = 0; i < len; ++i)
    {
      if (data[i] == key)
        return static_cast<int>(i);
    }
    return -1;
  }

  /**
   * @brief Remove an element by its index.
   *
   * This function removes the element at the specified index and shifts the subsequent elements
   * to fill the gap. If the index is invalid, an exception is thrown.
   *
   * @param index The index of the element to remove.
   * @throws std::out_of_range If the index is out of bounds.
   */
  void removeByIndex(size_t index)
  {
    if (index >= len)
    {
      throw std::out_of_range("Index out of bounds in Vector::removeByIndex");
    }

    for (size_t i = index; i < len - 1; ++i)
    {
      data[i] = data[i + 1];
    }

    --len;
  }
};
