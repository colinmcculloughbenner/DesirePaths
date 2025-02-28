#ifndef DESIREPATHS_GRID_H
#define DESIREPATHS_GRID_H

#include <memory>

struct GridCoordinate{
	int x;
	int y;

	bool operator==(const GridCoordinate& other) const {
		return x == other.x && y == other.y;
	}
};

template<>
struct std::hash<GridCoordinate> {
	std::size_t operator()(const GridCoordinate& c) const noexcept {
		auto h1 = std::hash<int>{}(c.x);
		auto h2 = std::hash<int>{}(c.y);
		return h1 ^ (h2 << 1);
	}
};

template <typename T>
class Grid {
public:
	Grid(unsigned int width, unsigned int height);
	T& GetData(GridCoordinate c);
	T& GetData(int x, int y);
	[[nodiscard]] unsigned int GetWidth() const { return m_width; }
	[[nodiscard]] unsigned int GetHeight() const { return m_height; }
private:
	[[nodiscard]] unsigned int gridCoordinateToIndex(int x, int y) const;
	[[nodiscard]] unsigned int gridCoordinateToIndex(GridCoordinate coordinate) const;

	unsigned int m_width;
	unsigned int m_height;
	std::unique_ptr<T[]> m_backingArray;
};

template<typename T>
Grid<T>::Grid(unsigned int width, unsigned int height) : m_width(width), m_height(height)
{
	m_backingArray = std::make_unique<T[]>(m_width * m_height);
}

template<typename T>
T& Grid<T>::GetData(GridCoordinate c)
{
	return m_backingArray[gridCoordinateToIndex(c)];
}

template<typename T>
T& Grid<T>::GetData(int x, int y)
{
	return m_backingArray[gridCoordinateToIndex(x, y)];
}

template<typename T>
unsigned int Grid<T>::gridCoordinateToIndex(int x, int y) const
{
	return x + y * m_width;
}

template<typename T>
unsigned int Grid<T>::gridCoordinateToIndex(GridCoordinate coordinate) const
{
	return gridCoordinateToIndex(coordinate.x, coordinate.y);
}

#endif //DESIREPATHS_GRID_H
