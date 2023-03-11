#pragma once

template <typename T>
bool equal(const T& a, const T& b) {
	return a == b;
}

template <typename T, typename... Args>
bool equal(const T& a, const T& b, const Args& ... rest) {
	return a == b && equal(b, rest...);
}