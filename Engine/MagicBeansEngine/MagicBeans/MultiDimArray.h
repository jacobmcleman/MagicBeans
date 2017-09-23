#pragma once

namespace Beans
{
  template<typename T>
  class Volume
  {
  public:
    Volume(size_t x, size_t y, size_t z)
    {
      data_ = new T**[x];
      for (int i = 0; i < x; ++i)
      {
        data_[i] = new T*[y];
        for (int j = 0; j < y; ++j)
        {
          data_[i][j] = new T[z];
        }
      }
    }

    T& At(size_t x, size_t y, size_t z)
    {
      return data_[x][y][z];
    }

    const T& At(size_t x, size_t y, size_t z) const
    {
      return data[x][y][z];
    }

  private:
    T*** data_;
  };
}