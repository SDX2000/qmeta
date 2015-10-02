#include "utils.h"

QStringRef mid(QStringRef lhs, QStringRef rhs)
{
    if(rhs.size() == 0 && rhs.position() == 0) {
        //HACK? This usually happens when advance() advances the string-ref to
        //the end of the string. Return full lhs in this case.
        return lhs;
    }

    return lhs.string()->midRef(lhs.position(), rhs.position() - lhs.position());
}
