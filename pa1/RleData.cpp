#include "RleData.h"
#include <iostream>
#include <math.h> 

void RleData::Compress(const char* input, size_t inSize)
{
	// A run ends when we counter two adjacent elements that are different
	delete[] mData;
	mSize = 0;
	mData = new char[2 * inSize];
	char maxSize = static_cast<char>(MaxRunSize());

	size_t index = 0; // index where every run starts at
	bool sign = true; // to indicate whether it is a positive or negative sign

	// Special cases
	if (inSize == 1)
	{
		mData[mSize] = 1;
		mSize += 1;
		mData[mSize] = input[0];
	}
	else if (inSize == 2)
	{
		if (input[0] == input[1])
		{
			mData[mSize] = 2;
			mSize += 1;
			mData[mSize] = input[0];
		}
		else
		{
			mData[mSize] = -2;
			mSize += 1;
			mData[mSize] = input[0];
			mSize += 1;
			mData[mSize] = input[1];
		}
	}
	else
	{ 
		for (size_t size = 0; size < inSize - 2; size++)		
		{
			
			//Positive runs
			if (input[size] == input[size + 1])				// if we see AA, then it is a positive run
			{
				sign = true;
				if (input[size + 1] != input[size + 2])		// if it is AAB, then the second A is the end of a positive run
				{
					while ((size + 2 - index) > 127)		// split long positive run
					{
						mData[mSize] = maxSize;
						mSize += 1;
						mData[mSize] = input[size];
						mSize += 1;
						index += 127;
					}
					mData[mSize] = static_cast<char>(size + 2 - index);
					mSize += 1;
					mData[mSize] = input[size];
					mSize += 1;
					index = size + 2;
					//special case: after adding two to the index, index > inSize - 3
					if (index == inSize - 2)
					{
						if (input[inSize - 2] == input[inSize - 1])
						{
							mData[mSize] = 2;
							mSize += 1;
							mData[mSize] = input[inSize - 2];
						}
						else
						{
							mData[mSize] = -2;
							mSize += 1;
							mData[mSize] = input[inSize - 2];
							mSize += 1;
							mData[mSize] = input[inSize - 1];
						}
					}
					if (size + 2 == inSize - 1)
					{
						mData[mSize] = 1;
						mSize += 1;
						mData[mSize] = input[size + 2];
					}
				}
				if (input[size + 1] == input[size + 2] && (size + 3) == inSize) // if a positive run reaches the end
				{
					while ((size + 3 - index) > 127)
					{
						mData[mSize] = maxSize;
						mSize += 1;
						mData[mSize] = input[size];
						mSize += 1;
						index += 127;
					}
					mData[mSize] = static_cast<char>(size + 3 - index);
					mSize += 1;
					mData[mSize] = input[size];
				}
			}

			else if (input[size] != input[size + 1])
			{
				if (size == 0)								// If starts from the beginning, it is a negative run
				{
					sign = false;
					index = 0;
				}
				else if (size != 0 && sign == true && input[size + 1] != input[size + 2]) //switch from a positive run to a negative run, index already set when the positive run ends
				{
					sign = false;
				}
			
				else if (sign == true && (size + 1 == inSize - 1))					// the file ends with AAB
				{
					mData[mSize] = -2;
					mSize += 1;
					mData[mSize] = input[size];
					mSize += 1;
					mData[mSize] = input[size + 1];
				}
				else if (sign == false && input[size + 1] == input[size + 2]) // aabcc
				{
					if ((index - size - 1) == -1)
					{
						mData[mSize] = 1;
						mSize += 1;
						mData[mSize] = input[index];
						mSize += 1;
						index += 1;
						sign = true;
						continue;
					}
					while ((size + 1 - index) > 127)					// split long negative run
					{
						mData[mSize] = -127;
						mSize += 1;
						for (size_t j = 0; j < MaxRunSize(); j++)
						{
							mData[mSize] = input[index + j];
							mSize += 1;
						}
						index += 127;
					}
					mData[mSize] = static_cast<char>(index - size - 1);
					mSize += 1;
					for (size_t i = index; i <= size; i++)
					{
						mData[mSize] = input[i];
						mSize += 1;
					}
					index = size + 1;
				}
				else if (sign == false && input[size + 1] != input[size + 2] && (size + 2 == inSize - 1))  // end with a negative run
				{
					while ((size + 3 - index) > 127)
					{
						mData[mSize] = -127;
						mSize += 1;
						for (size_t j = 0; j < MaxRunSize(); j++)
						{
							mData[mSize] = input[index + j];
							mSize += 1;
						}
						index += 127;
					}
					mData[mSize] = static_cast<char>(index - size - 3);					// split long negative run
					mSize += 1;
					for (size_t i = index; i < size + 3; i++)
					{
						mData[mSize] = input[i];
						mSize += 1;
					}
				}
			}
		}

	}
	


	/*
	OLD ALGORITHM

		for (size_t size = 2; size < inSize; size++)
		{
			if (input[size] == input[size - 1]) // if it is AA
			{
				if (input[size - 2] != input[size - 1]) // if it is B before AA
				{
					
					if (sign == true)					// if it is BAA and B is the end of a positive run
					{
						mData[mSize] = static_cast<char>(size - 1 - index);
						mSize += 1;
						mData[mSize] = input[size - 2];
						mSize += 1;
						index = size - 1;
					}
					else if (sign == false)								// if it is BAA and B is the end of a negative run
					{
						mData[mSize] = static_cast<char>(1 + index - size);
						mSize += 1;
						for (size_t i = index; i < size - 1; i++)
						{
							mData[mSize] = input[i];
							mSize += 1;
						}
						index = size - 1;
					}
				}
				if (size = inSize - 1)								//ABB and we reach the end: add the positive run of 2B's
				{
					mData[mSize] = 2;
					mSize += 1;
					mData[mSize] = input[size];
					mSize += 1;
				}
				sign = true;
			}
			else if (input[size] != input[size - 1]) // AB
			{
				
				if (input[size - 2] == input[size - 1] && size == inSize - 1) // AAB at the end
				{ 
					mData[mSize] = static_cast<char>(size - index);
					mSize += 1;
					mData[mSize] = input[size - 1];
					mSize += 1;
					mData[mSize] = 1;
					mData[mSize] = input[size];
				}
				
				if (input[size - 2] != input[size - 1])
				{
					sign = false;
					if (size == inSize - 1)				// ABC at the end
					{
						mData[mSize] = static_cast<char>(1 + index - size);
						mSize += 1;
						for (size_t i = index; i < size; i++)
						{
							mData[mSize] = input[i];
							mSize += 1;
						}
					}
				}
			}
		}
	*/
}


/*

	
	delete[] mData;
	mSize = 0;
	mData = new char[2 * inSize];
	size_t maxSize = MaxRunSize();
	bool isNegative = false; // true if the run is negative
	unsigned int positiveRunLength = 1; //The minimum length of positive run is 1
	int negativeRunLength = 2; //The minimum length of negative run is 2
	size_t negativeRunStartingIndex = 0; //Index where the negative run starts

	if (inSize == 1)
	{
		mData[0] = 1;
		mData[1] = input[0];
	}
	else if (inSize == 2)
	{
		if (input[0] == input[1])
		{
			mData[0] = 2;
			mData[1] = input[0];
		}
		else
		{
			mData[0] = -2;
			mData[1] = input[0];
			mData[2] = input[1];
		}
	}
	else
	{ 
	for (size_t size = 0; size < inSize - 2; size++)
	{
		if (input[size] == input[size + 1] && input[size + 1] == input[size + 2]) // AAA
		{
			isNegative = false;
			positiveRunLength += 1;
			if (size == inSize - 3)
			{
				positiveRunLength += 1;
				if (positiveRunLength > MaxRunSize())      // if the length is more than 127
				{
					int numOfRuns = static_cast<int>(floor(positiveRunLength / MaxRunSize())); // count how many short runs we need to divide the long run into
					for (int i = 0; i < numOfRuns; i++)
					{
						mData[mSize] = 127;
						mSize += 1;
						mData[mSize] = input[size + 1];
						mSize += 1;
						positiveRunLength -= MaxRunSize();
					}
					mData[mSize] = positiveRunLength;
					mSize += 1;
					mData[mSize] = input[size + 1];
					mSize += 1;
				}
				else if (positiveRunLength <= MaxRunSize())
				{
					mData[mSize] = positiveRunLength;
					mSize += 1;
					mData[mSize] = input[size + 1];
					mSize += 1;
				}
			}
		}
		else if (input[size] == input[size + 1] && input[size + 1] != input[size + 2]) // AAB
		{
			isNegative = false;
			positiveRunLength += 1;
			if (positiveRunLength > MaxRunSize())      // if the length is more than 127
			{
				int numOfRuns = static_cast<int>(floor(positiveRunLength / MaxRunSize())); // count how many short runs we need to divide the long run into
				for (int i = 0; i < numOfRuns; i++)
				{
					mData[mSize] = 127;
					mSize += 1;
					mData[mSize] = input[size + 1];
					mSize += 1;
					positiveRunLength -= MaxRunSize();
				}
				mData[mSize] = positiveRunLength;
				mSize += 1;
				mData[mSize] = input[size + 1];
				mSize += 1;
				if (size == inSize - 3) // if reaches the end
				{
					mData[mSize] = 1;
					mSize += 1;
					mData[mSize] = input[inSize - 1];
				}
			}
			else if (positiveRunLength <= MaxRunSize())
			{
				mData[mSize] = positiveRunLength;
				mSize += 1;
				mData[mSize] = input[size + 1];
				mSize += 1;
				positiveRunLength = 0; // Reset the length

				if (size == inSize - 3) // if reaches the end
				{
					mData[mSize] = 1;
					mSize += 1;
					mData[mSize] = input[inSize - 1];
				}
			}
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

				if (negativeRunLength > static_cast<int>(MaxRunSize()))      // if the length is more than 127
				{
					int numOfRuns = static_cast<int>(floor(negativeRunLength / MaxRunSize())); // count how many short runs we need to divide the long run into
					for (int i = 0; i < numOfRuns; i++)
					{ 
						mData[mSize] = -127;
						mSize += 1;
						for (size_t j = negativeRunStartingIndex; j < negativeRunStartingIndex + 127; j++)
						{
						mData[mSize] = input[j];
						mSize += 1;
						}
						negativeRunStartingIndex += 127;
						negativeRunLength -= MaxRunSize();
					}
					mData[mSize] = -negativeRunLength;
					mSize += 1;
					for (size_t i = negativeRunStartingIndex; i <= size; i++)
					{ 
						mData[mSize] = input[i];
						mSize += 1;
					}
				}
				else
				{ 
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
			}
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
		else if (input[size] != input[size + 1] && input[size + 1] != input[size + 2] && input[size] != input[size + 2]) // ABC
		{
			if (isNegative == false && size != 0) // if the previous run is positive, change the bool to negative and record the starting index
			{						 // AAABCEDFFFF the starting index is on B
				if (input[size + 2] != input[size + 3])
				{
					isNegative = true;
					negativeRunStartingIndex = size + 1;
					negativeRunLength = 2;
				}
				else if (input[size + 2] == input[size + 3]) // edgeCase AABCCDEEF
				{
					mData[mSize] = 1;
					mSize += 1;
					mData[mSize] = input[size + 1];
					mSize += 1;
				}
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

				if (negativeRunLength > static_cast<int>(MaxRunSize()))      // if the length is more than 127
				{
					int numOfRuns = static_cast<int>(floor(negativeRunLength / MaxRunSize())); // count how many short runs we need to divide the long run into
					for (int i = 0; i < numOfRuns; i++)
					{
						mData[mSize] = -127;
						mSize += 1;
						for (size_t j = negativeRunStartingIndex; j < negativeRunStartingIndex + 127; j++)
						{
							mData[mSize] = input[j];
							mSize += 1;
						}
						negativeRunStartingIndex += 127;
						negativeRunLength -= MaxRunSize();
					}
					mData[mSize] = -negativeRunLength;
					mSize += 1;
					for (size_t i = negativeRunStartingIndex; i <= inSize - 1; i++)
					{
						mData[mSize] = input[i];
						mSize += 1;
					}
				}
				else
				{
					mData[mSize] = -negativeRunLength;
					mSize += 1;
					for (size_t i = negativeRunStartingIndex; i <= inSize - 1; i++)
					{
						mData[mSize] = input[i];
						mSize += 1;
					}
				}
			}
			
		}
		//edge cases
		else if (input[size] != input[size + 1] && input[size + 1] != input[size + 2] && input[size] == input[size] + 2) // ABA
		{
			if (input[size - 1] == input[size] && input[size + 2] == input[size + 3])
			{
				mData[mSize] = 1;
				mSize += 1;
				mData[mSize] = input[size + 1];
				mSize += 1;
				positiveRunLength = 1;
			}
			else if (input[size - 1] != input[size] || input[size + 2] != input[size + 3])
			{
				negativeRunLength += 1;
			}
		
		}
		
	}
	
	}
	
}
*/

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	delete[] mData; //Delete data on the heap
	mSize = 0;
	mData = new char[outSize]; // Allocate memory for mData
	size_t index = 0;
	while (index < inSize)
	{
		int count = static_cast<int>(input[index]);
		if (count > 0)
		{
			for (int i = 1; i <= count; i++)
			{
				mData[mSize] = input[index + 1];
				mSize += 1;
			}
			index = index + 2;
		}
		else if (count < 0)
		{
			count *= (-1);
			for (int i = 1; i <= count; i++)
			{
				mData[mSize] = input[index + i];
				mSize += 1;
			}
			index = index + count + 1;
		}
	}
}


std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}