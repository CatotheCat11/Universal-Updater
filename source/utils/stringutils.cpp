/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019-2021 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "common.hpp"
#include "stringutils.hpp"
#include <charconv>
#include <stdarg.h>

/*
	To lowercase conversion.

	const std::string &str: The string which should be converted.
*/
std::string StringUtils::lower_case(const std::string &str) {
	std::string lower;
	transform(str.begin(), str.end(), std::back_inserter(lower), tolower); // Transform the string to lowercase.

	return lower;
}

/*
	Fetch strings from a vector and return it as a single string.

	std::vector<std::string> fetch: The vector.
*/
std::string StringUtils::FetchStringsFromVector(const std::vector<std::string> &fetch) {
	std::string temp;

	if (fetch.size() < 1) return ""; // Smaller than 1 --> Return empty.

	for (int i = 0; i < (int)fetch.size(); i++) {
		if (i != (int)fetch.size() - 1) {
			temp += fetch[i] + ", ";

		} else {
			temp += fetch[i];
		}
	}

	return temp;
}

/*
	adapted from GM9i's byte parsing.
*/
std::string StringUtils::formatBytes(u64 bytes) {
	char out[32];

	if (bytes == 1)					snprintf(out, sizeof(out), "%lld Byte", bytes);
	else if (bytes < 1ull << 10)	snprintf(out, sizeof(out), "%lld Bytes", bytes);
	else if (bytes < 1ull << 20)	snprintf(out, sizeof(out), "%.1f KiB", (float)bytes / 1024);
	else if (bytes < 1ull << 30)	snprintf(out, sizeof(out), "%.1f MiB", (float)bytes / 1024 / 1024);
	else if (bytes < 1ull << 40)	snprintf(out, sizeof(out), "%.1f GiB", (float)bytes / 1024 / 1024 / 1024);
	else							snprintf(out, sizeof(out), "%.1f TiB", (float)bytes / 1024 / 1024 / 1024 / 1024);

	return out;
}

/*
	Return a vector of all marks.
*/
std::vector<std::string> StringUtils::GetMarks(int marks) {
	std::vector<std::string> out;

	if (marks & favoriteMarks::STAR)	out.push_back( "★" );
	if (marks & favoriteMarks::HEART)	out.push_back( "♥" );
	if (marks & favoriteMarks::DIAMOND) out.push_back( "♦" );
	if (marks & favoriteMarks::CLUBS)	out.push_back( "♣" );
	if (marks & favoriteMarks::SPADE)	out.push_back( "♠" );

	return out;
}

/*
	Return a string of all marks.
*/
std::string StringUtils::GetMarkString(int marks) {
	std::string out;

	if (marks & favoriteMarks::STAR)	out += "★";
	if (marks & favoriteMarks::HEART)	out += "♥";
	if (marks & favoriteMarks::DIAMOND) out += "♦";
	if (marks & favoriteMarks::CLUBS)	out += "♣";
	if (marks & favoriteMarks::SPADE)	out += "♠";

	return out;
}

std::string StringUtils::format(const char *fmt_str, ...) {
	va_list ap;
	char *fp = nullptr;
	va_start(ap, fmt_str);
	vasprintf(&fp, fmt_str, ap);
	va_end(ap);

	std::unique_ptr<char, decltype(free) *> formatted(fp, free);
	return std::string(formatted.get());
}

/*
	Parse a color hex string (`#RRGGBB`) and return a C2D color.
	Return 0 if parsing failed.
*/
uint32_t StringUtils::ParseColorHexString(std::string_view str) {
	if (str.size() != 7) return 0;
	if (str[0] != '#') return 0;

	uint8_t colorVal[3];
	for (int i = 0; i < 3; i++) {
		const char *colorValStart = str.data() + 1 + i * 2;
		auto result = std::from_chars(colorValStart, colorValStart + 2, colorVal[i], 16);
		if (result.ec != std::errc()) return 0;
		if (result.ptr != colorValStart + 2) return 0;
	}

	return C2D_Color32(colorVal[0], colorVal[1], colorVal[2], 255);
}