#pragma once

#include "Wall.h"
class Tile;
class Corridor : public Wall {
	private:
		
		float  m_length;
		float  difx, dify;
		
	public:
		Wall m_upperLine;
		Wall m_lowerLine;
		Wall* m_upperExit;
		Wall* m_lowerExit;
		float  m_curX;
		float  m_curY;
		Corridor();
		Corridor(BOUNDINGLINE b1, BOUNDINGLINE b2, Wall* lowerExit, Wall* upperExit, Tile *tiles);
		void SetCorridor(BOUNDINGLINE b1, BOUNDINGLINE b2, Wall* lowerExit, Wall* upperExit, Tile *tiles);
		float GetLength() const;
		void RotateCorridor(float angle);
		void TranslateCorridor(float x, float y);
		void Render() const;
};