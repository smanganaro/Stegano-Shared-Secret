
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>

/*
 * Offsets & sizes (in bytes) of the BMP header
 * where useful information can be found.
 */
#define WIDTH_HEADER_OFFSET 0x12
#define WIDTH_HEADER_SIZE 0x4

#define HEIGHT_HEADER_OFFSET 0x16
#define HEIGHT_HEADER_SIZE 0x4

// Amount bits per pixel
#define BITS_PER_PIXEL_HEADER_OFFSET 0x1C
#define BITS_PER_PIXEL_HEADER_SIZE 0x2

// Compression
#define COMPRESSION_HEADER_OFFSET 0x1E
#define COMPRESSION_HEADER_SIZE 0x4

#define BODY_START_HEADER_OFFSET 0x0A
#define BODY_START_HEADER_SIZE 0x4

#define PIXEL_SIZE 1

// Size of a byte
#define BYTE 0x1

typedef uint64_t u_int64_t;
typedef uint32_t u_int32_t; 
typedef uint16_t u_int16_t; 
typedef uint8_t u_int8_t; 

typedef enum mode_t
{
    DISTRIBUTE,
    RETRIEVE
} Mode;


// Stores all the relevant BMP header information
typedef struct header_t
{
    // Size in pixels
    u_int32_t width;
    u_int32_t height;

    u_int16_t bits_per_pixel;

    // Has compression unless value is 0
    u_int32_t compression;

    // The location of the start of the image information relative to the start of the file
    u_int32_t offset;

    int _padding_bytes;

} BMPHeader;

typedef struct byte_buffer_t
{
    u_int8_t *start;
    u_int32_t length;
} ByteBuffer;

typedef struct pixel_t
{
    u_int8_t value;
} Pixel;

typedef struct pixel_node_t {
    Pixel pixel;
    struct pixel_node_t * next;
} PixelNode;


typedef struct options_t
{
    Mode mode;
    char *secret_file_name;
    char *water_mark_file_name;
    u_int8_t min_shadows_amount;
    u_int8_t total_amount_of_shadows;
    char *directory;

} Options;