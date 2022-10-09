#pragma once

class TextureData
{
public:

	TextureData(char const* filename, int* x, int* y, int* comp, int req_comp);
	~TextureData();

	/**
	 * Get whether data was succesfully loaded.
	 * 
	 * \return whether data was succesfully loaded.
	 */
	bool hasData();

	/**
	 * Get this texture's data.
	 * 
	 * \return this texture's data.
	 */
	unsigned char* getData();

	/**
	 * Get the reason for any stbi failures.
	 * 
	 * \return the reason for any stbi failures.
	 */
	const char* getFailureReason();

private:

	unsigned char* data;

};

