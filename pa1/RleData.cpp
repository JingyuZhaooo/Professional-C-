#include "RleData.h"
#include <iostream>

void RleData::Compress(const char* input, size_t inSize)
{
	delete[] mData;
	mSize = 0;
	mData = new char[2 * inSize];
	size_t maxSize = MaxRunSize();

	
	bool isNegative = false; // true if the run is negative
	unsigned int positiveRunLength = 1; //The minimum length of positive run is 1
	int negativeRunLength = 2; //The minimum length of negative run is 2
	size_t negativeRunStartingIndex = 0; //Index where the negative run starts
	

	for (size_t size = 0; size < inSize - 2; size++)
	{
		if (input[size] == input[size + 1] && input[size + 1] == input[size + 2]) // AAA
		{
			isNegative = false;
			positiveRunLength += 1;
			if (size == inSize - 3)
			{
				positiveRunLength += 1;
				mData[mSize] = positiveRunLength;
				mSize += 1;
				mData[mSize] = input[size + 1];
				mSize += 1;
			}
		}
		else if (input[size] == input[size + 1] && input[size + 1] != input[size + 2]) // AAB
		{
			isNegative = false;
			positiveRunLength += 1;
			mData[mSize] = positiveRunLength;
			mSize += 1;
			mData[mSize] = input[size + 1];
			mSize += 1;
			positiveRunLength = 0; // Reset the length
		}
		else if (input[size] != input[size + 1] && input[size + 1] == input[size + 2]) // ABB
		{
			if (size == 0)			// Edge Case: ABB from the start
			{
				mData[mSize] = 1;
				mSize += 1;
				mData[mSize] = input[size];
				mSize += 1;
			}
			if (isNegative == false) // if the previous run is positive, then ABB means the start of another positive run
			{
				positiveRunLength += 1;
			}
			else if (isNegative == true) // switch from a negative run to a positive run
			{
				negativeRunLength -= 1; // Because we did not know we are going to swicth or not, substract the one previous added
				mData[mSize] = -negativeRunLength;
				mSize += 1;
				for (size_t i = negativeRunStartingIndex; i <= size; i++)
				{
					mData[mSize] = input[i];
					mSize += 1;
				}
				negativeRunLength = 2; // Reset the length
				positiveRunLength = 1; // Start of a positive run
				isNegative = false;
			}

			/*
			if (isNegative == true) // case of AABCC
			{
				isNegative = false;
				mData[mSize] = 1;
				mSize += 1;
				mData[mSize] = input[size];
				mSize += 1;
			}
			*/

			else					 // if we are in a negative run, then ABB means the run stops
			{						 // if we have AAABCDEFFFF
				mData[mSize] = -negativeRunLength;
				mSize += 1;
				for (size_t i = negativeRunStartingIndex; i <= size; i++)
				{
					mData[mSize] = input[i];
					mSize += 1;
				}
			}
			
		}

		//for negative runs
		else if (input[size] != input[size + 1] && input[size + 1] != input[size + 2] && input[size] != input[size] + 2) // ABC
		{
			if (isNegative == false && size != 0) // if the previous run is positive, change the bool to negative and record the starting index
			{						 // AAABCEDFFFF the starting index is on B
				isNegative = true;
				negativeRunStartingIndex = size + 1;
				negativeRunLength = 2;
			}
			else if (isNegative == false && size == 0) // negative run from the start
			{
				negativeRunLength += 1;
				isNegative = true;
				negativeRunStartingIndex = size;
			}
			else if (isNegative == true && size != inSize - 3)                        // in the middle of a negative run
			{
				negativeRunLength += 1;
			}
			else if (isNegative == true && size == inSize - 3) // reach the end
			{							// ABCDEFGH
				negativeRunLength += 1;
				mData[mSize] = -negativeRunLength;
				mSize += 1;
				for (size_t i = negativeRunStartingIndex; i <= inSize - 1; i++)
				{
					mData[mSize] = input[i];
					mSize += 1;
				}
			}
			
		}
		

		//edge cases
		else if (input[size] != input[size + 1] && input[size + 1] != input[size + 2] && input[size] == input[size] + 2) // ABA
		{
			mData[mSize] = 1;
			mSize += 1;
			mData[mSize] = input[size + 1];
			mSize += 1;
			positiveRunLength = 1;
		}
		
	}
	
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// TODO
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}