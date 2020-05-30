#pragma once

class ImageBits {
public:
	using Byte = unsigned char;

	struct Pixel {
		Byte b, g, r, a;

		Pixel() :
			r{}, g{}, b{}, a{} {};

		Pixel(Byte red, Byte green, Byte blue, Byte alpha = 255) :
			r{red}, g{green}, b{blue}, a{alpha} {};
	};

	enum class BlendMode {
		Replace,
		Alpha,
		Add
	};

private:
	Pixel *_begin{}, *_end{};
	int width{}, height{};
	bool created_from_imageio{};

	ImageBits(Pixel* begin, Pixel* end, int width, int height) :
		_begin{begin}, _end{end}, width{width}, height{height} {}

public:
	ImageBits() noexcept {};
	ImageBits(ImageBits&&) noexcept;
	ImageBits& operator=(ImageBits&&) noexcept;
	virtual ~ImageBits();

	Pixel* begin() noexcept { return _begin; }
	Pixel* end() noexcept { return _end; }
	const Pixel* cbegin() const noexcept { return _begin; }
	const Pixel* cend() const noexcept { return _end; }
	int Width() const noexcept { return width; }
	int Height() const noexcept { return height; }
	Pixel* GetPixel(int x, int y);
	const Pixel* GetPixel(int x, int y) const;

private:
	inline ImageBits& Move(ImageBits&) noexcept;
	void Destroy();

public:
	static Pixel CreatePixel(Byte red, Byte green, Byte blue, Byte alpha = 0xff);
	static Byte* AllocBits(int width, int height);
	static void FreeBits(Byte* bits);
	static ImageBits CreateFromBits(int width, int height, Byte* bits, bool created_from_imageio = false);
	static ImageBits CreateEmpty(int width, int height);
	static ImageBits LoadFile(const char* filename);

	void SaveFile(const char* filename);
	void Clear(const Pixel& color);
	void Fill(int x, int y, int w, int h, const Pixel& color, BlendMode blend_mode = BlendMode::Replace);
	void Fill(int x, int y, const ImageBits& source, BlendMode blend_mode = BlendMode::Replace);

private:
	bool AdjustClip(int& x, int& y, int& x2, int& y2, int w, int h);
	void Blend(Pixel& dest, const Pixel& src, BlendMode blend_mode);
};

inline bool operator==(const ImageBits::Pixel&, const ImageBits::Pixel&);
