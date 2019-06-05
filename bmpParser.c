#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "stego.h"

int get_bmp_row_size(BMPHeader *header);

BMPHeader *parse_header_from_bytebuffer(ByteBuffer *byte_buffer)
{
  BMPHeader *header = malloc(sizeof(BMPHeader));

  if (header == NULL)
  {
    return NULL;
  }

  u_int32_t width;
  u_int32_t height;
  u_int16_t bits_per_pixel;
  u_int32_t compression;
  u_int32_t offset;

  memcpy(&width, byte_buffer->start + WIDTH_HEADER_OFFSET, WIDTH_HEADER_SIZE);
  memcpy(&height, byte_buffer->start + HEIGHT_HEADER_OFFSET, HEIGHT_HEADER_SIZE);
  memcpy(&bits_per_pixel, byte_buffer->start + BITS_PER_PIXEL_HEADER_OFFSET, BITS_PER_PIXEL_HEADER_SIZE);
  memcpy(&compression, byte_buffer->start + COMPRESSION_HEADER_OFFSET, COMPRESSION_HEADER_SIZE);
  memcpy(&offset, byte_buffer->start + BODY_START_HEADER_OFFSET, BODY_START_HEADER_SIZE);

  header->width = width;
  header->height = height;
  header->bits_per_pixel = bits_per_pixel;
  header->compression = compression;
  header->offset = offset;

  header->_padding_bytes = get_bmp_row_size(header) - header->width * PIXEL_SIZE;

  return header;
}

void write_header_to_bytebuffer(BMPHeader *header, ByteBuffer *byte_buffer)
{
  u_int32_t width = header->width;
  u_int32_t height = header->height;
  u_int16_t bits_per_pixel = header->bits_per_pixel;
  u_int32_t compression = header->compression;
  u_int32_t offset = header->offset;

  memcpy(byte_buffer->start + WIDTH_HEADER_OFFSET, &width, WIDTH_HEADER_SIZE);
  memcpy(byte_buffer->start + HEIGHT_HEADER_OFFSET, &height, HEIGHT_HEADER_SIZE);
  memcpy(byte_buffer->start + BITS_PER_PIXEL_HEADER_OFFSET, &bits_per_pixel, BITS_PER_PIXEL_HEADER_SIZE);
  memcpy(byte_buffer->start + COMPRESSION_HEADER_OFFSET, &compression, COMPRESSION_HEADER_SIZE);
  memcpy(byte_buffer->start + BODY_START_HEADER_OFFSET, &offset, BODY_START_HEADER_SIZE);
}

int is_compressed(BMPHeader *header)
{
  return header->compression != 0;
}

/**
 *  get_bmp_row_size
 *
 *  Returns the size of the bmp row in bytes.
 */
int get_bmp_row_size(BMPHeader *header)
{
  return ((int)floor((header->bits_per_pixel * header->width + 31) / 32.0)) * 4;
}

/**
 *  get_bmp_body_size
 *
 *  Returns the size of the bmp body in bytes.
 */
int get_bmp_body_size(BMPHeader *header)
{
  return  get_bmp_row_size(header) * header->height;
}

ByteBuffer *infer_body(char *filename, BMPHeader *header)
{
  FILE *fd;
  ByteBuffer *byte_buffer = calloc(sizeof(ByteBuffer), 1);

  byte_buffer->length = get_bmp_body_size(header);
  byte_buffer->start = malloc(byte_buffer->length);

  if (byte_buffer->start == NULL)
  {
    return NULL;
  }

  if ((fd = fopen(filename, "rb")) == NULL)
  {
    return NULL;
  }

  fseek(fd, header->offset, SEEK_SET);
  if (fread(byte_buffer->start, byte_buffer->length, BYTE, fd) != BYTE)
  {
    return NULL;
  }

  return byte_buffer;
}

ByteBuffer *infer_header(char *filename)
{
  ByteBuffer *byte_buffer = malloc(sizeof(ByteBuffer));

  FILE *fd;
  if ((fd = fopen(filename, "rb")) == NULL)
  {
    return NULL;
  }

  fseek(fd, BODY_START_HEADER_OFFSET, SEEK_SET);
  u_int32_t offset;
  if (fread(&offset, BODY_START_HEADER_SIZE, BYTE, fd) != BYTE)
  {
    return NULL;
  }

  byte_buffer->length = offset;
  byte_buffer->start = malloc(byte_buffer->length);

  // Go to the beginning of the file
  fseek(fd, 0, SEEK_SET);
  if (fread(byte_buffer->start, 1, byte_buffer->length, fd) != (size_t)byte_buffer->length)
  {
    return NULL;
  }

  fclose(fd);
  return byte_buffer;
}

/**
 *  get_as_reserved_pixel_list
 *
 *  Returns a reversed simple list of pixels, ignoring the padding.
 */
PixelNode *infer_reversed_pixel_list(BMPHeader *header, ByteBuffer *body)
{
  PixelNode * first = malloc(sizeof(PixelNode));
  PixelNode * curr = first;
  int row_size = get_bmp_row_size(header);

  for (int i = 0; i < (int)header->height; i++)
  {
    for (unsigned int j = 0; j < header->width; j++)
    {
      int curr_pos = i * row_size + j * PIXEL_SIZE;
      u_int32_t pixel_bytes = 0;

      memcpy(&pixel_bytes, body->start + curr_pos, 1);
      curr->pixel.value = pixel_bytes & 0xFF;
      

      if (i != (int)header->height - 1 || j != header->width - 1)
      {
        curr->next = malloc(sizeof(PixelNode));
      }
      curr = curr->next;
    }

  }

  return first;
}

ByteBuffer * create_body(BMPHeader *header, PixelNode *file_with_message)
{
  ByteBuffer * buffer = calloc(BYTE, sizeof(ByteBuffer));

  buffer->length = get_bmp_body_size(header);
  buffer->start = calloc(BYTE, buffer->length);
  u_int32_t row_length_bytes = get_bmp_row_size(header);

  PixelNode * curr = file_with_message;

  for (int i = 0; i < (int)header->height; i++)
  {
    for (u_int32_t j = 0; j < header->width; j++)
    {
      u_int32_t pixel_offset = row_length_bytes * i + j * PIXEL_SIZE;
      memcpy(buffer->start + pixel_offset, &curr->pixel.value, BYTE);
      curr = curr->next;
    }

    int zero = 0x0;
    memcpy(buffer->start + row_length_bytes * (i + 1) - header->_padding_bytes, &zero, header->_padding_bytes);
  }

  return buffer;
}