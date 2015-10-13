#pragma once

namespace AeonUI
{
	class Point
	{
	public:
		Point();
		Point(int x, int y);
		~Point() {};
		
		int x;
		int y;
	};
}