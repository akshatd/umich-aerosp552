#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

double get_value_from_cin(std::string value_name, double min, double max);

constexpr double kRadiusEarth = 3440;
void             fly(double latA, double longA, double latB, double longB);

constexpr double kToRadCoeff = M_PI / 180.0;
double           to_rad(double deg);

constexpr double kToDegCoeff = 180.0 / M_PI;
double           to_deg(double rad);

int main(void) {
	// double latA = 51.5007;
	// double longA = 0.1246;
	// double latB = 40.6892;
	// double longB = 74.0445;

	double latA  = get_value_from_cin("latA", 0, 360);
	double longA = get_value_from_cin("longA", 0, 360);
	double latB  = get_value_from_cin("latB", 0, 360);
	double longB = get_value_from_cin("longB", 0, 360);
	fly(latA, longA, latB, longB);

	return 0;
}

double get_value_from_cin(std::string value_name, double min, double max) {
	bool   input_valid = false;
	double input;
	while (!input_valid) {
		std::cout << "Enter " << value_name << ": ";
		if (std::cin >> input) {
			if (input < min) {
				std::cout << "** Error, " << value_name << " must be >= " << min << "\n";
			} else if (input >= max) {
				std::cout << "** Error, " << value_name << " must be < " << max << "\n";
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

	std::cout << "a= " << a << ", d= " << d << ", theta= " << theta << '\n';
	std::cout << "Fly heading " << theta << " degrees for " << d << " nautical miles.\n";
}

double to_rad(double deg) { return deg * kToRadCoeff; }

double to_deg(double rad) { return rad * kToDegCoeff; }