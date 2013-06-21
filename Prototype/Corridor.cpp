#include "Corridor.h"
//////////////////////////////////////////////////////////////////////////////////
/// Default Constructor.
Corridor::Corridor() { m_length = 0; }
//////////////////////////////////////////////////////////////////////////////////
/// Constructor which calls Corridor::SetCorridor
Corridor::Corridor(BOUNDINGLINE b1, BOUNDINGLINE b2, Wall* lowerExit, Wall* upperExit, Tile *tiles) {
	SetCorridor(b1, b2, lowerExit, upperExit, tiles);
}
//////////////////////////////////////////////////////////////////////////////////
/// Creates a corridor by creating two wall segments
void Corridor::SetCorridor(BOUNDINGLINE b1, BOUNDINGLINE b2, Wall* lowerExit, Wall* upperExit, Tile *tiles) {
	m_upperLine.UseTiles(tiles, 1);
	m_lowerLine.UseTiles(tiles, 1);

	m_upperLine.CreateWallSegment(b1.x1, b1.y1, b1.x2, b1.y2);
	m_lowerLine.CreateWallSegment(b2.x1, b2.y1, b2.x2, b2.y2);

	m_lowerExit = lowerExit;
	m_upperExit = upperExit;

}
//////////////////////////////////////////////////////////////////////////////////
/// Returns the length of this corridor
float Corridor::GetLength() const { return m_length; }
//////////////////////////////////////////////////////////////////////////////////
/// Rotates the corridor by the specified angle.
void Corridor::RotateCorridor(float angle) {
	m_upperLine.Rotate(angle);
	m_lowerLine.Rotate(angle);
}
//////////////////////////////////////////////////////////////////////////////////
/// Renders the corridor.
void Corridor::Render() const {
	m_upperLine.Render();
	m_lowerLine.Render();
}
//////////////////////////////////////////////////////////////////////////////////
/// Moves the corridor by a translation vector
void Corridor::TranslateCorridor(float x, float y) {
	m_upperLine.TranslateWall(x, y);
	m_lowerLine.TranslateWall(x, y);
}