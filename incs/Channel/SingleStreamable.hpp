#ifndef SINGLESTREAMABLE_HPP
# define SINGLESTREAMABLE_HPP

#include <Channel/Channel.hpp>
#include "../../libs/shared_ptr/shared_ptr.hpp"

class SingleStreamable {
protected:
    ft::shared_ptr<Channel> _channel;
public:
    int getFd(void) const;
    ft::shared_ptr<Channel> getChannel(void) const;
private:
    SingleStreamable(const SingleStreamable &ref);
    SingleStreamable &operator=(const SingleStreamable &rhs);
public:
    virtual ~SingleStreamable(void);
protected:
    SingleStreamable(ft::shared_ptr<Channel> channel);
    SingleStreamable(void);
};

#endif