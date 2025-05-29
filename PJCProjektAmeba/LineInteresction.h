#pragma once
#include"Vec2i.h"
#include<cmath>

bool checkIfLinesIntersect(const Vec2i& posA, const Vec2i& posB, const Vec2i& posC, const Vec2i& posD) {

	Vec2i AB = (posB - posA);
	Vec2i AC = (posC - posA);
	Vec2i AD = (posD - posA);

	Vec2i CD = (posD - posC);
	Vec2i CA = (posA - posC);
	Vec2i CB = (posB - posC);

	int ABxAC = AB.cross(AC);
	int ABxAD = AB.cross(AD);

	int CDxCA = CD.cross(CA);
	int CDxCB = CD.cross(CB);

	if ((ABxAC > 0 && ABxAD < 0) || (ABxAC < 0 && ABxAD > 0) || (CDxCA > 0 && CDxCB < 0) || (CDxCA < 0 && CDxCB > 0)) {
		return true;
	}

	if (ABxAC == 0 && endOn(posA, posC, posB)) return true;
	if (ABxAD == 0 && endOn(posA, posD, posB)) return true;
	if (CDxCA == 0 && endOn(posC, posA, posD)) return true;
	if (CDxCB == 0 && endOn(posC, posB, posD)) return true;

	return false;
}

bool endOn(const Vec2i& a, const Vec2i& b, const Vec2i& c) {
	return std::min(a.getXi(), c.getXi()) <= b.getXi() && b.getXi() <= std::max(a.getXi(), c.getXi()) &&
		std::min(a.getYi(), c.getYi()) <= b.getYi() && b.getYi() <= std::max(a.getYi(), c.getYi());
}
