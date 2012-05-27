#ifndef SIMPLE_CIRCULAR_BUFFER_H
#define SIMPLE_CIRCULAR_BUFFER_H

template <typename T>
class SimpleCircularBuffer
{
public:
	SimpleCircularBuffer (int size) {
		wrapped = false;

		size_full = 2*size;
		size_part = size;

		part1_current = 0;
		part2_current = size_part;

		buffer = new T[size_full];

		for (int xx = 0; xx < size_full; xx++) {
			buffer[xx] = (T) 0;
		}
	}

	~SimpleCircularBuffer() {
		delete[] buffer;
		buffer = NULL;
	}

	void ResetBuffer()  {
		wrapped = false;
		part1_current = 0;
		part2_current = size_part;
	}

	void PushToBuffer (T value) {

		buffer[part1_current] = value;
		buffer[part2_current] = value;

		part1_current++;
		part2_current++;

		if (size_part == part1_current) {
			wrapped = true;
			part1_current = 0;
			part2_current = size_part;
		}
	}

	T *GetBuffer () {

		if (wrapped) {
			return &buffer[part1_current];
		}

		return &buffer[0];
	}

private:
	bool wrapped;

	int size_full;
	int size_part;

	int part1_current;
	int part2_current;

	T *buffer;
};

#endif /* SIMPLE_CIRCULAR_BUFFER_H */
