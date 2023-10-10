#ifndef DUALSTREAMABLE_HPP
# define DUALSTREAMABLE_HPP

#include <Channel/CgiChannel.hpp>
#include "../../libs/shared_ptr/shared_ptr.hpp"

class DualStreamable {

protected:
    ft::shared_ptr<CgiChannel> _cgiChannel;
public:
    int getFd(CgiChannel::e_pipe pipe) const;
    ft::shared_ptr<CgiChannel> getCgiChannel(void) const;
private:
    DualStreamable(void);
    DualStreamable(const DualStreamable &ref);
    DualStreamable &operator=(const DualStreamable &rhs);
protected:
    DualStreamable(ft::shared_ptr<CgiChannel> channel);
};

#endif