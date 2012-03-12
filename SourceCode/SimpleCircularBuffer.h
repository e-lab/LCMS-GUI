#ifndef SIMPLE_CIRCULAR_BUFFER_H
#define SIMPLE_CIRCULAR_BUFFER_H

template <typename T>
class SimpleCircularBuffer
{
public:
	SimpleCircularBuffer (int size) {
		rapped = false;

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

	void PushToBuffer (T value) {

		buffer[part1_current] = value;
		buffer[part2_current] = value;

		part1_current++;
		part2_current++;

		if (size_part == part1_current) {
			rapped = true;
			part1_current = 0;
			part2_current = size_part;
		}
	}

	T *GetBuffer () {

		if (rapped) {
			return &buffer[part1_current];
		}

		return &buffer[0];
	}

private:
	bool rapped;

	int size_full;
	int size_part;

	int part1_current;
	int part2_current;

	T *buffer;
};

#endif /* SIMPLE_CIRCULAR_BUFFER_H */
