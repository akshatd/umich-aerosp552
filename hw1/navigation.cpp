#include <iostream>
#include <cmath>
#include <string>

// from google maps
constexpr double kMaxLat  = 90;
constexpr double kMaxLong = 180;

double get_value_from_cin(std::string value_name, double min, double max);

constexpr double kRadiusEarth = 3440;
void             fly(double latA, double longA, double latB, double longB);

constexpr double kToRadCoeff = M_PI / 180.0;
constexpr double to_rad(double deg);

constexpr double kToDegCoeff = 180.0 / M_PI;
constexpr double to_deg(double rad);

int main(void) {
	// test for helper functions
	static_assert(to_deg(M_PI) == 180.0);
	static_assert(to_deg(M_PI / 2) == 90.0);
	static_assert(to_deg(M_PI * 2) == 360.0);
	static_assert(to_deg(0) == 0);
	static_assert(to_deg(-M_PI / 2) == -90.0);
	static_assert(to_rad(180.0) == M_PI);
	static_assert(to_rad(90.0) == M_PI / 2);
	static_assert(to_rad(360.0) == M_PI * 2);
	static_assert(to_rad(0) == 0);
	static_assert(to_rad(-90.0) == -M_PI / 2);

	// tests: distance verified from https://www.vcalc.com/wiki/vcalc/haversine-distance
	// slight difference due to nautical mile conversion or precision of earth's radius
	fly(51.5007, 0.1246, 40.6892, 74.0445); // d = 3010.17818574514
	fly(-90, -180, 90, 180);                // d = 10807.3002159827
	fly(90, 180, -90, -180);                // d = 10807.3002159827
	fly(90, 180, 90, 180);                  // d = 0
	fly(-90, -180, -90, -180);              // d = 0
	fly(0, 0, 0, 0);                        // d = 0

	std::cout << '\n';
	double latA  = get_value_from_cin("latA", -kMaxLat, kMaxLat);
	double longA = get_value_from_cin("longA", -kMaxLong, kMaxLong);
	double latB  = get_value_from_cin("latB", -kMaxLat, kMaxLat);
	double longB = get_value_from_cin("longB", -kMaxLong, kMaxLong);
	fly(latA, longA, latB, longB);

	return 0;
}

// tested with inputs out of range and garbage inputs like strings
double get_value_from_cin(std::string value_name, double min, double max) {
	bool   input_valid = false;
	double input;
	while (!input_valid) {
		std::cout << "Enter " << value_name << ": ";
		if (std::cin >> input) {
			if (input < min) {
				std::cout << "** Error, " << value_name << " must be >= " << min << "\n";
			} else if (input > max) {
				std::cout << "** Error, " << value_name << " must be <= " << max << "\n";
			} else {
				input_valid = true;
			}
		} else {
			std::cout << "** Error, bad input\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}
	return input;
}

void fly(double latA, double longA, double latB, double longB) {
	latA     = to_rad(latA);
	longA    = to_rad(longA);
	latB     = to_rad(latB);
	longB    = to_rad(longB);
	double a = pow(sin((latB - latA) / 2), 2) + cos(latA) * cos(latB) * pow(sin((longB - longA) / 2), 2);
	double d = 2 * kRadiusEarth * atan2(sqrt(a), sqrt(1. - a));
	double theta =
		to_deg(atan2(sin(longB - longA) * cos(latB), cos(latA) * sin(latB) - sin(latA) * cos(latB) * cos(longA - longB)));

	// std::cout << "a= " << a << ", d= " << d << ", theta= " << theta << '\n';
	std::cout << "Fly heading " << theta << " degrees for " << d << " nautical miles.\n";
}

constexpr double to_rad(double deg) { return deg * kToRadCoeff; }

constexpr double to_deg(double rad) { return rad * kToDegCoeff; }