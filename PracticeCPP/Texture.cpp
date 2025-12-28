#include "pch.h"
#include "Texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{
	if (_bitmap)
	{
		::DeleteObject(_bitmap);
		_bitmap = nullptr;
	}

	if (_hdc)
	{
		::DeleteDC(_hdc);
		_hdc = nullptr;
	}
}

Texture* Texture::LoadBmp(HWND hwnd, const wstring& path)
{
	HDC hdc = ::GetDC(hwnd);

	_hdc = ::CreateCompatibleDC(hdc);
	_bitmap = (HBITMAP)::LoadImage(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	::ReleaseDC(hwnd, hdc);

	if (_bitmap == 0)
	{
		::MessageBox(hwnd, path.c_str(), L"Image Load Failed", NULL);
		::DeleteDC(_hdc);
		_hdc = nullptr;
		return this;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(_hdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);
	_size.x = bit.bmWidth;
	_size.y = bit.bmHeight;

	return this;
}

HDC Texture::GetDC()
{
	return _hdc;
}