#ifndef DUALSTREAMABLE_HPP
# define DUALSTREAMABLE_HPP

#include <Channel/CgiChannel.hpp>
#include "../../libs/shared_ptr/shared_ptr.hpp"

class DualStreamable {

protected:
    ft::shared_ptr<CgiChannel> _cgiChannel;
public:
    int getDualFd(e_pipe pipe) const;
    ft::shared_ptr<CgiChannel> getCgiChannel(void) const;
private:
    DualStreamable(const DualStreamable &ref);
    DualStreamable &operator=(const DualStreamable &rhs);
public:
    virtual ~DualStreamable(void);
protected:
    DualStreamable(ft::shared_ptr<CgiChannel> cgiChannel);
    // void buildCgiChannel();
};

#endif