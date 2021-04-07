#include "C:/Newbie/Samples/Lofi\Scripts\RotateScript.hpp"
#include "C:/Newbie/Samples/Lofi\UserExport.hpp"
#include <type_traits>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
void RotateScript::Serialize(json &js, const Engine::Entity *entity) {
  Engine::Script::StaticType()->Serialize(js, entity);
  const RotateScript *e = (const RotateScript *)entity;
  js.push_back(e->rate);
}
void RotateScript::Deserialize(json &js, Engine::Entity *entity) {
  RotateScript *e = (RotateScript *)entity;
  e->rate = js.back().get<float>(); js.erase(--js.end());
  Engine::Script::StaticType()->Deserialize(js, entity);
}
void type_init() {
  RotateScript::StaticType(new Engine::Type("RotateScript", Engine::instantiate<RotateScript, true>, RotateScript::Serialize, RotateScript::Deserialize));
}
void type_clear() {
  delete RotateScript::StaticType();
}
