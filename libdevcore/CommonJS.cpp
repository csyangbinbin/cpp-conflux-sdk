// Aleth: Ethereum C++ client, tools and libraries.
// Copyright 2014-2019 Aleth Authors.
// Licensed under the GNU General Public License, Version 3.


#include "CommonJS.h"

using namespace std;

namespace dev
{

bytes jsToBytes(string const& _s, OnFailed _f)
{
	try
	{
		return fromHex(_s, WhenError::Throw);
	}
	catch (...)
	{
		if (_f == OnFailed::InterpretRaw)
			return asBytes(_s);
		else if (_f == OnFailed::Throw)
			throw invalid_argument("Cannot intepret '" + _s + "' as bytes; must be 0x-prefixed hex or decimal.");
	}
	return bytes();
}

bytes padded(bytes _b, unsigned _l)
{
	while (_b.size() < _l)
		_b.insert(_b.begin(), 0);
	return asBytes(asString(_b).substr(_b.size() - max(_l, _l)));
}

bytes paddedRight(bytes _b, unsigned _l)
{
	_b.resize(_l);
	return _b;
}

bytes unpadded(bytes _b)
{
	auto p = asString(_b).find_last_not_of((char)0);
	_b.resize(p == string::npos ? 0 : (p + 1));
	return _b;
}

bytes unpadLeft(bytes _b)
{
	unsigned int i = 0;
	if (_b.size() == 0)
		return _b;

	while (i < _b.size() && _b[i] == uint8_t(0))
		i++;

	if (i != 0)
		_b.erase(_b.begin(), _b.begin() + i);
	return _b;
}

string fromRaw(h256 _n)
{
	if (_n)
	{
		string s((char const*)_n.data(), 32);
		auto l = s.find_first_of('\0');
		if (!l)
			return "";
		if (l != string::npos)
			s.resize(l);
		for (auto i: s)
			if (i < 32)
				return "";
		return s;
	}
	return "";
}

}

namespace dev {

	Address jsToAddress(std::string const& _s) {
		auto b = dev::fromHex(_s.substr(0, 2) == "0x" ? _s.substr(2) : _s, dev::WhenError::DontThrow);
		if (b.size() == 20)
			return Address(b);
		else
			return Address(dev::bytes());

	}

	std::string prettyU256(u256 _n, bool _abridged)
	{
		std::string raw;
		std::ostringstream s;
		if (!(_n >> 64))
			s << " " << (uint64_t)_n << " (0x" << std::hex << (uint64_t)_n << ")";
		else if (!~(_n >> 64))
			s << " " << (int64_t)_n << " (0x" << std::hex << (int64_t)_n << ")";
		else if ((_n >> 160) == 0)
		{
			Address a = right160(_n);

			std::string n;
			if (_abridged)
				n = a.abridged();
			else
				n = toHex(a.ref());

			if (n.empty())
				s << "0";
			else
				s << _n << "(0x" << n << ")";
		}
		else if (!(raw = fromRaw((h256)_n)).empty())
			return "\"" + raw + "\"";
		else
			s << "" << (h256)_n;
		return s.str();
	}

}
