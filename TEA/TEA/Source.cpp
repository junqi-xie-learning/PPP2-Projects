#include "../../std_lib_facilities.h"

void encipher(
	const unsigned long *const v,
	unsigned long *const w,
	const unsigned long * const k)
{
	unsigned long y = v[0];
	unsigned long z = v[1];
	unsigned long sum = 0;
	unsigned long delta = 0x9E3779B9;
	unsigned long n = 32;
	while (n-- > 0) {
		y += (z << 4 ^ z >> 5) + z ^ sum + k[sum & 3];
		sum += delta;
		z += (y << 4 ^ y >> 5) + y ^ sum + k[sum >> 11 & 3];
	}
	w[0] = y; w[1] = z;
}

void decipher(
	const unsigned long *const v,
	unsigned long *const w,
	const unsigned long * const k)
{
	unsigned long y = v[0];
	unsigned long z = v[1];
	unsigned long sum = 0xC6EF3720;
	unsigned long delta = 0x9E3779B9;
	unsigned long n = 32;

	while (n-- > 0) {
		z -= (y << 4 ^ y >> 5) + y ^ sum + k[sum >> 11 & 3];
		sum -= delta;
		y -= (z << 4 ^ z >> 5) + z ^ sum + k[sum & 3];
	}
	w[0] = y; w[1] = z;
}

int main()
try {
	const int nchar = 2 * sizeof(long);
	const int kchar = 2 * nchar;

	string op;
	string key;
	string infile;
	string outfile;
	cout << "please enter input file name, output file name, and key:\n";
	cin >> infile >> outfile >> key;
	while (key.size() < kchar) key += '0';
	ifstream inf(infile);
	ofstream outf(outfile);
	if (!inf || !outf) error("bad file name");

	const unsigned long* k =
		reinterpret_cast<const unsigned long*>(key.data());

	unsigned long outptr[2];
	char inbuf[nchar];
	unsigned long* inptr = reinterpret_cast<unsigned long*>(inbuf);
	int count = 0;

	while (inf.get(inbuf[count])) {
		outf << showbase << hex;
		if (++count == nchar) {
			encipher(inptr, outptr, k);
			outf << setw(8) << setfill('0') << outptr[0] << ' '
				<< setw(8) << setfill('0') << outptr[1] << ' ';
			count = 0;
		}
	}

	if (count) {
		while (count != nchar) inbuf[count++] = '0';
		encipher(inptr, outptr, k);
		outf << outptr[0] << ' ' << outptr[1] << ' ';
	}
}
catch (exception& e) {
	cerr << "error: " << e.what() << '\n';
	char ch; cin >> ch;
	return 1;
}
catch (...) {
	cerr << "error: unknown error\n";
	char ch; cin >> ch;
	return -1;
}