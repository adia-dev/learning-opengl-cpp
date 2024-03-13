#pragma once

#include <string>
class Texture2D {
public:
  Texture2D(const std::string &file_path);
  ~Texture2D();

  void bind(unsigned int slot = 0) const;
  void unbind() const;

  unsigned int ID() const;
  unsigned char* local_buffer() const;
  int width() const;
  int height() const;
  int bpp() const;
  const std::string &file_path() const;

private:
  unsigned int _id;
  unsigned char* _pixel_data;
  int _width;
  int _height;
  int _bpp;
  std::string _file_path;
};
