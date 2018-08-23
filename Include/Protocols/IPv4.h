#ifndef IPV4_H
#define IPV4_H

#include <netinet/ip.h>
#include <vector>

class IPv4
{
    public:
        IPv4();
        ~IPv4();

        bool decodePackage(std::vector<unsigned char>* aBuffer);
        void clear();

        unsigned char version() const;
        unsigned char headerLength() const;

        unsigned long sourceAddress() const;
        unsigned long destinationAddress() const;

        std::vector<unsigned char>* getPakageData() const;

    private:
        static const unsigned MIN_IP_PACKAGE_LENGTH = 20;
        static const unsigned MAX_IP_PACKAGE_LENGTH = 65535;

    private:
        bool readHeader(std::vector<unsigned char>* aBuffer);

        bool needToReadHeaderOptions();
        bool readHeaderOptions(std::vector<unsigned char>* aBuffer);
        unsigned headerOptionLength(const unsigned aIpHeaderLength) const;

        bool readData(std::vector<unsigned char>* aBuffer);
        unsigned dataLength(const unsigned aIpPackageTotalLength);

    private:
        iphdr m_ipHeader;

        std::vector<unsigned char>* m_data;
        std::vector<unsigned char> m_headerOptions;

        bool m_isHeaderDecoded;
};

#endif // IPV4_H
