/*
 * Passphrase to 40(64)- and 104(128)-bit WEP key generator.
 * 
 * The following code was put together from information found at
 * 
 * - http://security.stackexchange.com/questions/35614/standard-algorithm-for-wep-key-generator-64-bit
 * - http://wiki.buici.com/xwiki/bin/view/Systems+Administration/WEP+Key+Generator
 * 
 * Functionality was verified against http://www.powerdog.com/wepkey.cgi
 * 
 * No originality or copyright is claimed on this code.
 * 
 * In jurisdictions that recognize copyright laws, the author
 * of this software dedicate any and all copyright interest in the
 * software to the public domain.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org/>.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <openssl/md5.h>

int main( int argc, char **argv )
{
	uint8_t key64[ 4 ][ 5 ] = {
		{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }
	};
	uint8_t seed[ 4 ] = { 0, 0, 0, 0 };
	uint32_t randNumber;
	MD5_CTX ctx;
	unsigned char key128[ MD5_DIGEST_LENGTH ];
	unsigned int i, j;
	const char *passphrase;

	if( argc != 2 )
	{
		fprintf( stderr, "Need one passphrase argument\n" );
		return 1;
	}

	passphrase = argv[ 1 ];

	for( i = 0; i < strlen( passphrase ); i++ )
	{
		seed[ i % 4 ] ^= ( uint8_t )( passphrase[ i ] );
	}

	randNumber = ( uint32_t )( seed[ 0 ] | ( seed[ 1 ] << 8 ) | ( seed[ 2 ] << 16 ) | ( seed[ 3 ] << 24 ) );

	for( i = 0; i < 4; i++ )
	{
		for( j = 0; j < 5; j++ )
		{
			randNumber = ( randNumber * 0x0343FD + 0x269EC3 ) & 0xFFFFFFFF;
			key64[ i ][ j ] = ( randNumber >> 16 ) & 0xFF;
		}
	}

	for( i = 0; i < 4; i++ )
	{
		printf( "%d.  40-bit key: 0x", i + 1 );
		for( j = 0; j < 5; j++)
		{
			printf( "%02X", key64[ i ][ j ] );
		}
		puts( "" );
	}

	j = 64;
	MD5_Init( &ctx );
	while( j > strlen( passphrase ) )
	{
		MD5_Update( &ctx, passphrase, strlen( passphrase ) );
		j -= strlen( passphrase );
	}

	if( j > 0 )
	{
		MD5_Update( &ctx, passphrase, j );
	}

	MD5_Init(&ctx);

	j = 64;
	while( j >= strlen( passphrase ) )
	{
		MD5_Update( &ctx, passphrase, strlen( passphrase ) );
		j -= strlen( passphrase );
	}

	if( j > 0 )
	{
		MD5_Update( &ctx, passphrase, j );
	}

	MD5_Final( key128, &ctx );

	printf( "   104-bit key: 0x" );
	for( i = 0; i < 13; i++ )
	{
		printf( "%02X", ( uint8_t )key128[ i ] );
	}
	puts( "" );

	return 0;
}

