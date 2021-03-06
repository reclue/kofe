#ifndef OBJECT_H
#define OBJECT_H

#include "interface/iprocess.h"
#include "transform/transformable.h"
#include "misc/collectionof.h"


class Object :
    public IProcess,
    public Transformable {
protected:
    CollectionOf<Object> m_children;

public:
    friend void swap(Object&, Object&);

public:
    Object();
    Object(const Object& other);
    Object(Object&& other) noexcept;
    Object& operator=(const Object& other);
    Object& operator=(Object&& other) noexcept;
    virtual ~Object();

public:
    CollectionOf<Object>& Children();
    const CollectionOf<Object>& Children() const;

public: /* IProcess */
    void Processing() override;
};


void swap(Object& lhs, Object& rhs);

#endif // OBJECT_H
