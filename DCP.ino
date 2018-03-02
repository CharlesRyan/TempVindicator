// from misc file. may or may not apply to bch
BYTE CalcByteCksum(BYTE* data, BYTE length)
{
  BYTE ret = 0;

  while ( length-- ) 
  {
    ret += *data; // accumulate checksum
    data++;
  }

  return (ret);
}
