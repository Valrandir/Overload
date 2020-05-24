#pragma once

class ImageBits {
public:
	struct Pixel {
		unsigned char b, g, r, a;
	};

private:
	Pixel *_begin{}, *_end{};
	int width{}, height{};

public:
	ImageBits() {}
	ImageBits(ImageBits&&) noexcept;
	ImageBits& operator=(ImageBits&&) noexcept;
	virtual ~ImageBits();

	[[nodiscard]] Pixel* begin() noexcept { return _begin; }
	[[nodiscard]] Pixel* end() noexcept { return _end; }
	[[nodiscard]] const Pixel* cbegin() const noexcept { return _begin; }
	[[nodiscard]] const Pixel* cend() const noexcept { return _end; }
	[[nodiscard]] int GetWidth() const noexcept { return width; }
	[[nodiscard]] int GetHeight() const noexcept { return height; }

private:
	inline ImageBits& Move(ImageBits&) noexcept;

public:
	Pixel* Alloc(int width, int height);
	void Free();
};
