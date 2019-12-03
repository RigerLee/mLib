
namespace ml {

	//! returns the time in seconds
	double Timer::getTime()	{
		struct timeval timevalue;
		gettimeofday(&timevalue, nullptr);
		return (double)((UINT64)timevalue.tv_sec) + (double)((UINT64)timevalue.tv_usec) / 1000000.0;
	}
} // namespace ml
