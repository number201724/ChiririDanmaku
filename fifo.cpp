#include "pch.h"
#include "fifo.h"

CFifoBuf::CFifoBuf( void *buffer, size_t maxsize )
{
	_cursize = 0;
	_wpos = 0;
	_rpos = 0;
	_maxsize = maxsize;
	_buffer = (unsigned char *)buffer;
}

bool CFifoBuf::front( void *data, size_t len )
{
	size_t lastlen = len;
	size_t tmplen;
	unsigned char *dst;
	size_t rpos;
	size_t cursize;

	if( _cursize < len )
		return false;

	cursize = _cursize;
	rpos = _rpos;
	dst = (unsigned char *)data;
	do
	{
		tmplen = (_maxsize - rpos);

		if( tmplen > lastlen )
		{
			memcpy( &dst[len - lastlen], &_buffer[rpos], lastlen );
			rpos += lastlen;
			cursize -= lastlen;
			return true;
		}
		else
		{
			memcpy( &dst[len - lastlen], &_buffer[rpos], tmplen );
			lastlen -= tmplen;
			rpos = 0;
			cursize -= tmplen;
		}
	} while( lastlen > 0 );

	return true;
}

void CFifoBuf::pop( size_t len )
{
	size_t lastlen = len;
	size_t tmplen;

	if( _cursize < len )
	{

		return;
	}

	do
	{
		tmplen = (_maxsize - _rpos);

		if( tmplen > lastlen )
		{
			_rpos += lastlen;
			_cursize -= lastlen;
			break;
		}
		else
		{
			lastlen -= tmplen;
			_rpos = 0;
			_cursize -= tmplen;
		}

	} while( lastlen > 0 );
}

bool CFifoBuf::deque( void *data, size_t len )
{
	size_t lastlen = len;
	size_t tmplen;
	unsigned char *dst;

	if( _cursize < len )
		return false;

	dst = (unsigned char *)data;
	do
	{
		tmplen = (_maxsize - _rpos);

		if( tmplen > lastlen )
		{
			memcpy( &dst[len - lastlen], &_buffer[_rpos], lastlen );
			_rpos += lastlen;
			_cursize -= lastlen;
			return true;
		}
		else
		{
			memcpy( &dst[len - lastlen], &_buffer[_rpos], tmplen );
			lastlen -= tmplen;
			_rpos = 0;
			_cursize -= tmplen;
		}

	} while( lastlen > 0 );

	return true;
}

bool CFifoBuf::queue( const void *data, size_t len )
{
	const unsigned char *indata;
	size_t copylen;
	size_t newlen = (len + _cursize);

	indata = (unsigned char *)data;

	//OVERFLOW CHECK
	if( newlen > _maxsize )
	{
		return false;
	}

	copylen = 0;

	do
	{
		size_t endsize = (_maxsize - _wpos);

		if( endsize > (len - copylen) )
		{
			memcpy( &_buffer[_wpos], &indata[copylen], len - copylen );
			_wpos += (len - copylen);
			_cursize += (len - copylen);
			return true;
		}
		else
		{
			memcpy( &_buffer[_wpos], &indata[copylen], endsize );
			_cursize += endsize;
			_wpos = 0;
			copylen += endsize;
		}
	} while( (len - copylen) > 0 );

	return true;
}

void CFifoBuf::clear( )
{
	_cursize = 0;
	_wpos = 0;
	_rpos = 0;
}

size_t CFifoBuf::size( )
{
	return _cursize;
}

bool CFifoBuf::empty( )
{
	return _cursize == 0;
}