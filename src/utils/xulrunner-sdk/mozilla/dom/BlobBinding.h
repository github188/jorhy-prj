/* THIS FILE IS AUTOGENERATED - DO NOT EDIT */

#ifndef mozilla_dom_BlobBinding_h__
#define mozilla_dom_BlobBinding_h__

#include "mozilla/ErrorResult.h"
#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/DOMJSClass.h"
#include "mozilla/dom/DOMJSProxyHandler.h"

class XPCWrappedNativeScope;

namespace mozilla {
namespace dom {

namespace EndingTypesValues {

  enum valuelist {
    Transparent,
    Native
  };

  extern const EnumEntry strings[3];
} // namespace EndingTypesValues


typedef EndingTypesValues::valuelist EndingTypes;


struct BlobPropertyBagWorkers {
  BlobPropertyBagWorkers() {}
  bool Init(JSContext* cx, JSObject* scopeObj, const JS::Value& val);
  bool ToObject(JSContext* cx, JSObject* parentObject, JS::Value *vp);


  EndingTypes mEndings;
  nsString mType;
private:
  // Disallow copy-construction
  BlobPropertyBagWorkers(const BlobPropertyBagWorkers&) MOZ_DELETE;
  static jsid endings_id;
  static jsid type_id;
};
struct BlobPropertyBagWorkersInitializer : public BlobPropertyBagWorkers {
  BlobPropertyBagWorkersInitializer() {
    // Safe to pass a null context if we pass a null value
    Init(nullptr, nullptr, JS::NullValue());
  }
};

struct BlobPropertyBag : public MainThreadDictionaryBase {
  BlobPropertyBag() {}
  bool Init(JSContext* cx, JSObject* scopeObj, const JS::Value& val);
  bool ToObject(JSContext* cx, JSObject* parentObject, JS::Value *vp);

  bool Init(const nsAString& aJSON)
  {
    mozilla::Maybe<JSAutoRequest> ar;
    mozilla::Maybe<JSAutoCompartment> ac;
    jsval json = JSVAL_VOID;
    JSContext* cx = ParseJSON(aJSON, ar, ac, json);
    NS_ENSURE_TRUE(cx, false);
    return Init(cx, nullptr, json);
  }

  EndingTypes mEndings;
  nsString mType;
private:
  // Disallow copy-construction
  BlobPropertyBag(const BlobPropertyBag&) MOZ_DELETE;
  static bool InitIds(JSContext* cx);
  static bool initedIds;
  static jsid endings_id;
  static jsid type_id;
};
struct BlobPropertyBagInitializer : public BlobPropertyBag {
  BlobPropertyBagInitializer() {
    // Safe to pass a null context if we pass a null value
    Init(nullptr, nullptr, JS::NullValue());
  }
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_BlobBinding_h__
