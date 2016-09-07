#include "RleData.h"
#include <iostream>
#include <math.h> 

void RleData::Compress(const char* input, size_t inSize)
{/*
	delete[] mData;
	mSize = 0;
	mData = new char[2 * inSize];
	size_t maxSize = MaxRunSize();

	int runLength = 0;

	for (size_t counter = 0; counter < inSize; counter++)
	{

	}





	unsigned int positiveRunLength = 1;
	int negativeRunLength = 1;
	size_t runStartingIndex = 0;
	bool sign = true; // true means positive run, false means negative run
	bool turningPoint = false; // indicate whether it is a turning point between a positive run and a negative run
	char previous = input[0]; //Get the first element (head)
	size_t negativeRunStartIndex = 0; // index where the negative run starts
	int beginning = 0; // to indicate the run starts from the beginning

	if (inSize == 1) // case which there is only one char in the input
	{
		mData[mSize] = 1;
		mSize += 1;
		mData[mSize] = previous;
	}
	if (inSize > 1) // if there is more than one char in input
	{ 


		for (size_t size = 1; size < inSize; size++)
		{											// compare the current element to the previous element
			beginning++;
			if (input[size] == previous)			// if the current element == previous element, we have a positive run increasing
			{
				sign = true;			// in a positive run
				positiveRunLength += 1;
				if (size == inSize - 1) // if we reach the end
				{
					mData[mSize] = positiveRunLength;
					mSize += 1;
					mData[mSize] = previous;
				}
				if (turningPoint == true) // we switch from a positive run to another positive run
				{
					turningPoint = false; // set turning point back to false : it is not turning 
					sign = true;			// in a positive run
				}
				if (sign == false) //have switched from negative to positive
				{
					sign = true;
					negativeRunLength -= 1; // subtract the ectra count of char in the negative run
					mData[mSize] = -negativeRunLength;
					mSize += 1;
					for (size_t i = negativeRunStartIndex; i < size - 1; i++)
					{
						mData[mSize] = input[i];
						mSize += 1;
					}
				}
			}


			else if (input[size] != previous)	// we can only identify where the run stops at the point where adjacent elements are different
			{
				if (beginning == 1)
				{
					sign = false;
					negativeRunLength += 1;
					negativeRunStartIndex = 0;
					break;
				}
				if (turningPoint == false && sign == true) // a positive run just ends, and we need to record that run
				{
					mData[mSize] = positiveRunLength;
					mSize += 1;
					mData[mSize] = previous;
					mSize += 1;
					positiveRunLength = 1; // reset the length of positive run
					turningPoint = true; // we don't know the coming run is positive or negative yet, just set it to true first
				}
				else if (turningPoint == true && sign == true) // switching from positive to negative
				{
					sign = false; // set the sign to negative
					negativeRunLength += 1;
					turningPoint = false;
					negativeRunStartIndex = size - 1;
				}
				else if (turningPoint == false && sign == false) // in the middle of a negative run
				{
					negativeRunLength += 1;
				}
				
				else if (size == inSize - 1 && sign == true) // AAAAAAAAAB
				{

				}
			}
			previous = input[size]; // set the previous to the current element

			//EdgeCases:
			if (size == inSize - 1 && sign == false) // a negative run reaches the end
			{
				std::cout << "asdffvxc" << std::endl;
				mData[mSize] = -negativeRunLength;
				mSize += 1;
				for (unsigned i = negativeRunStartIndex; i < inSize; i++)
				{
					mData[mSize] = input[i];
					mSize += 1;
				}
			}
			
		}

	}
	*/
	
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
		else if (input[size] != input[size + 1] && input[size + 1] != input[size + 2] && input[size] == input[size] + 2) // abababab
		{
			mData[mSize] = 1;
			mSize += 1;
			mData[mSize] = input[size + 1];
			mSize += 1;
			positiveRunLength = 1;
		}
		
	}
	
	}
	
}

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