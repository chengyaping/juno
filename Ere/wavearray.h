#ifndef wavearray_h
#define wavearray_h
class wavearray
{


    public:

                        virtual  wavearray<double>(int)
                                 wavearray<double>()
                                 wavearray<double>(const wavearray<double>&)
       virtual wavearray<double> white(int = 1) const
                         virtual ~wavearray<double>()
                            void add(const wavearray<double>&, int = 0, int = 0, int = 0)
                      static int compare(const void* x, const void* y)
                            void cpf(const wavearray<double>&, int = 0, int = 0, int = 0)
                    virtual void Dump(const char*, int = 0)
                    virtual void DumpBinary(const char*, int = 0)
                    virtual void DumpShort(const char*, int = 0)
                    virtual void FFT(int = 1)
                          double getStatistics(double& mean, double& rms) const
                            void hann()
                  virtual size_t limit() const
                  virtual size_t limit(const slice& s) const
                  virtual size_t limit(const wavearray<double>& a) const
                  virtual double max() const
                  virtual double mean(double = -1.) const
                  virtual double mean(const slice&)
                  virtual double median(double** = NULL, size_t = 0, size_t = 0) const
                  virtual double min() const
      virtual wavearray<double>& operator*=(wavearray<double>&)
      virtual wavearray<double>& operator*=(const double)
      virtual wavearray<double>& operator+=(wavearray<double>&)
      virtual wavearray<double>& operator+=(const double)
      virtual wavearray<double>& operator-=(wavearray<double>&)
      virtual wavearray<double>& operator-=(const double)
      virtual wavearray<double>& operator<<(wavearray<double>&)
              wavearray<double>& operator=(const wavearray<double>&)
              wavearray<double>& operator=(const double)
      virtual wavearray<double>& operator[](const slice&)
                 virtual double& operator[](const unsigned int)
                            long rand48(long k = 1024)
                          double rank(double = 0.5) const
                    virtual void rate(double r)
                  virtual double rate() const
                    virtual void ReadBinary(const char*)
                    virtual void ReadShort(const char*)
                            void Resample(const wavearray<double>&, double, int = 6)
                            void resample(const wavearray<double>&, double, int = 6)
                    virtual void resample(double, int = 6)
                    virtual void resize(unsigned int)
                  virtual double rms()
                  virtual double rms(const slice&)
                  virtual size_t size() const
                          double Stack(const wavearray<double>&, int)
                          double Stack(const wavearray<double>&, int, int)
                          double Stack(const wavearray<double>&, double)
                    virtual void start(double s)
                  virtual double start() const
                            void sub(const wavearray<double>&, int = 0, int = 0, int = 0)
                            long uniform()
                virtual double** waveSort(double** = NULL, size_t = 0, size_t = 0) const
                    //virtual void waveSplit(double**, size_t, size_t, size_t) const


    public:

      double* data   data array
       size_t Size   number of elements in the data array
       double Rate   data sampling rate
       double Start  start time
        slice Slice  the data slice structure












         wavearray<double> wavearray(int)
         wavearray<double> wavearray()
         wavearray<double> wavearray(const wavearray<double>&)
        wavearray<double>& operator=(const wavearray<double>&)
        wavearray<double>& operator[](const slice&)
                   double& operator[](const unsigned int)
                    size_t limit() const
                    size_t limit(const slice& s) const
                    size_t limit(const wavearray<double>& a) const
        wavearray<double>& operator+=(wavearray<double>&)
        wavearray<double>& operator-=(wavearray<double>&)
        wavearray<double>& operator*=(wavearray<double>&)
        wavearray<double>& operator<<(wavearray<double>&)
        wavearray<double>& operator=(const double)
        wavearray<double>& operator+=(const double)
        wavearray<double>& operator-=(const double)
        wavearray<double>& operator*=(const double)
                      void Dump(const char*, int = 0)
                      void DumpBinary(const char*, int = 0)
                      void DumpShort(const char*, int = 0)
                      void ReadBinary(const char*)
                      void ReadShort(const char*)
                      void FFT(int = 1)
                      void Resample(const wavearray<double>&, double, int = 6)
                      void resample(const wavearray<double>&, double, int = 6)
                      void resample(double, int = 6)
                      void start(double s)
                    double start() const
                      void rate(double r)
                    double rate() const
                    size_t size() const
                    double median(double** = NULL, size_t = 0, size_t = 0) const
                    double mean(double = -1.) const
                    double mean(const slice&)
                    double rms()
                    double rms(const slice&)
                    double max() const
                    double min() const
                      void hann()
                  double** waveSort(double** = NULL, size_t = 0, size_t = 0) const
                      void waveSplit(double**, size_t, size_t, size_t) const
                    double rank(double = 0.5) const
         wavearray<double> white(int = 1) const
                      long uniform()
                      long rand48(long k = 1024)
                    double getStatistics(double& mean, double& rms) const
                      void resize(unsigned int)
                      void cpf(const wavearray<double>&, int = 0, int = 0, int = 0)
                      void add(const wavearray<double>&, int = 0, int = 0, int = 0)
                      void sub(const wavearray<double>&, int = 0, int = 0, int = 0)
                    double Stack(const wavearray<double>&, int)
                    double Stack(const wavearray<double>&, int, int)
                    double Stack(const wavearray<double>&, double)
                       int compare(const void* x, const void* y)
                      void ~wavearray()




  };
#endif 
