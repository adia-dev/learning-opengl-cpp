#include "texture_2d.h"

#include <renderer/renderer.h>
#include <renderer/utils.h>
#include <stb_image/stb_image.h>

Texture2D::Texture2D()
    : _id(0), _pixel_data(nullptr), _width(0), _height(0), _bpp(0) {}

Texture2D::Texture2D(const std::string &file_path)
    : _id(0), _pixel_data(nullptr), _width(0), _height(0), _bpp(0),
      _file_path(std::move(file_path)) {

  load();
}

Texture2D::~Texture2D() {
  if (_pixel_data) {
    stbi_image_free(_pixel_data);
    _pixel_data = nullptr;
  }

  GL_CALL(glDeleteTextures(1, &_id));
}

void Texture2D::load() {
  stbi_set_flip_vertically_on_load(1);
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  _pixel_data =
      stbi_load(_file_path.c_str(), &_width, &_height, &_bpp, STBI_rgb_alpha);

  if (_pixel_data == nullptr) {
    std::cout << "An error occured loading the image: `" + _file_path + "`.\n";
    return;
  }

  GL_CALL(glGenTextures(1, &_id));
  GL_CALL(glBindTexture(GL_TEXTURE_2D, _id));

  GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA,
                       GL_UNSIGNED_BYTE, _pixel_data));
  GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

  GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

  if (_pixel_data) {
    stbi_image_free(_pixel_data);
    _pixel_data = nullptr;
  }

  std::cout << "Texture2D: `" + _file_path + "` loaded.\n";
}

void Texture2D::bind(unsigned int slot) const {
  GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
  GL_CALL(glBindTexture(GL_TEXTURE_2D, _id));
}
void Texture2D::unbind() const { GL_CALL(glBindTexture(GL_TEXTURE_2D, 0)); }

unsigned int Texture2D::ID() const { return _id; }
unsigned char *Texture2D::local_buffer() const { return _pixel_data; }
int Texture2D::width() const { return _width; }
int Texture2D::height() const { return _height; }
int Texture2D::bpp() const { return _bpp; }
const std::string &Texture2D::file_path() const { return _file_path; }

void Texture2D::set_file_path(const std::string &file_path) {
  _file_path = file_path;
}
