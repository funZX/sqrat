//
// SqratObjectReference: Storage object for a class instance
//

//
// Copyright (c) 2017 COMP Omega <compomega@tutanota.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
//    2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
//    3. This notice may not be removed or altered from any source
//    distribution.
//

#if !defined(_SCRAT_OBJECTREFERENCE_H_)
#define _SCRAT_OBJECTREFERENCE_H_

#include <squirrel.h>
#include <memory>

#include "sqratUtil.h"

namespace Sqrat
{

/// @cond DEV
class ObjectReferenceBase
{
public:
    ObjectReferenceBase(bool owner = true) : mOwner(owner) {
        sq_resetobject(&squirrelObject);
    }

    virtual ~ObjectReferenceBase() { }

    virtual bool IsNull() const = 0;
    virtual bool IsShared() const = 0;
    virtual bool IsValid() const = 0;

    bool IsOwner() const {
        return mOwner;
    }

    void MakeOwner() {
        mOwner = false;
    }

    void SetSquirrelObject(HSQOBJECT obj) {
        squirrelObject = obj;
    }

    HSQOBJECT GetSquirrelObject() const {
        return squirrelObject;
    }

    HSQOBJECT* GetSquirrelObjectPtr() {
        return &squirrelObject;
    }

private:
    bool mOwner;
    HSQOBJECT squirrelObject;
};

template<class T>
class ObjectReference : public ObjectReferenceBase
{
public:
    ObjectReference(bool owner = true) : ObjectReferenceBase(owner),
        object(nullptr) { }

    virtual ~ObjectReference() {
        if(IsOwner()) {
            delete object;
        }
    }

    virtual bool IsNull() const {
        return nullptr == object && !sharedObject;
    }

    virtual bool IsShared() const {
        return nullptr != sharedObject.get();
    }

    virtual bool IsValid() const {
        return (nullptr == object && sharedObject) ||
            (nullptr != object && !sharedObject);
    }

    void SetObject(T *obj) {
        object = obj;
        sharedObject.reset();
    }

    T* GetObject() const {
        return object;
    }

    void SetSharedObject(const std::shared_ptr<T>& obj) {
        sharedObject = obj;
        object = nullptr;
    }

    T* GetSharedObject() const {
        return sharedObject;
    }

    std::shared_ptr<T> Promote() {
        if(!IsShared() && IsOwner())
        {
            sharedObject = std::make_shared<T>(object);
            object = nullptr;
        }

        return sharedObject;
    }

    T* GetPointer() const {
        if(IsShared()) {
            return sharedObject.get();
        } else {
            return object;
        }
    }

    T& GetReference() const {
        if(IsShared()) {
            return *sharedObject;
        } else {
            return *object;
        }
    }

    T& operator*() const noexcept {
        return GetReference();
    }

    T* operator->() const noexcept {
        return GetPointer();
    }

    explicit operator bool() const noexcept {
        return IsValid();
    }

private:
    T *object;
    std::shared_ptr<T> sharedObject;
};

/// @endcond

}

#endif
