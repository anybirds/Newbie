#include "c:/Newbie/Engine\Asset.hpp"
#include "c:/Newbie/Engine\Component.hpp"
#include "c:/Newbie/Engine\Engine.hpp"
#include "c:/Newbie/Engine\EngineExport.hpp"
#include "c:/Newbie/Engine\Entity.hpp"
#include "c:/Newbie/Engine\GameObject.hpp"
#include "c:/Newbie/Engine\Graphics\Camera.hpp"
#include "c:/Newbie/Engine\Graphics\Drawer.hpp"
#include "c:/Newbie/Engine\Graphics\Framebuffer.hpp"
#include "c:/Newbie/Engine\Graphics\Material.hpp"
#include "c:/Newbie/Engine\Graphics\Mesh.hpp"
#include "c:/Newbie/Engine\Graphics\MeshDrawer.hpp"
#include "c:/Newbie/Engine\Graphics\Model.hpp"
#include "c:/Newbie/Engine\Graphics\Renderer.hpp"
#include "c:/Newbie/Engine\Graphics\Shader.hpp"
#include "c:/Newbie/Engine\Graphics\Texture.hpp"
#include "c:/Newbie/Engine\Graphics\Window.hpp"
#include "c:/Newbie/Engine\Group.hpp"
#include "c:/Newbie/Engine\Project.hpp"
#include "c:/Newbie/Engine\Resource.hpp"
#include "c:/Newbie/Engine\Scene.hpp"
#include "c:/Newbie/Engine\Script.hpp"
#include "c:/Newbie/Engine\Time.hpp"
#include "c:/Newbie/Engine\Transform.hpp"
#include "c:/Newbie/Engine\Type.hpp"
#include <type_traits>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace Engine {
void Asset::Serialize(json &js, const Engine::Entity *entity) {
  Entity::StaticType()->Serialize(js, entity);
  const Asset *e = (const Asset *)entity;
  js.push_back(e->serial);
  js.push_back(e->name);
}
}
namespace Engine {
void Component::Serialize(json &js, const Engine::Entity *entity) {
  Entity::StaticType()->Serialize(js, entity);
  const Component *e = (const Component *)entity;
  js.push_back(e->localEnabled);
  js.push_back(e->enabled);
  js.push_back(e->gameObject);
}
}
namespace Engine {
void Entity::Serialize(json &js, const Engine::Entity *entity) {
  const Entity *e = (const Entity *)entity;
}
}
namespace Engine {
void GameObject::Serialize(json &js, const Engine::Entity *entity) {
  Entity::StaticType()->Serialize(js, entity);
  const GameObject *e = (const GameObject *)entity;
  js.push_back(e->name);
  js.push_back(e->group);
  js.push_back(e->transform);
  js.push_back(e->components);
}
}
namespace Engine {
void Camera::Serialize(json &js, const Engine::Entity *entity) {
  Renderer::StaticType()->Serialize(js, entity);
  const Camera *e = (const Camera *)entity;
  js.push_back(e->dirty);
  js.push_back(e->normalization);
  js.push_back(e->orthographic);
  js.push_back(e->fovy);
  js.push_back(e->aspectRatio);
  js.push_back(e->nr);
  js.push_back(e->fr);
  js.push_back(e->size);
  js.push_back(e->framebuffer);
}
}
namespace Engine {
void Drawer::Serialize(json &js, const Engine::Entity *entity) {
  Component::StaticType()->Serialize(js, entity);
  const Drawer *e = (const Drawer *)entity;
  js.push_back(e->order);
}
}
namespace Engine {
void AFramebuffer::Serialize(json &js, const Engine::Entity *entity) {
  Asset::StaticType()->Serialize(js, entity);
  const AFramebuffer *e = (const AFramebuffer *)entity;
  js.push_back(e->colorTexture);
  js.push_back(e->depthTexture);
}
}
namespace Engine {
void AMaterial::Serialize(json &js, const Engine::Entity *entity) {
  Asset::StaticType()->Serialize(js, entity);
  const AMaterial *e = (const AMaterial *)entity;
  js.push_back(e->vertexShader);
  js.push_back(e->fragmentShader);
  js.push_back(e->mainTexture);
}
}
namespace Engine {
void AMesh::Serialize(json &js, const Engine::Entity *entity) {
  Asset::StaticType()->Serialize(js, entity);
  const AMesh *e = (const AMesh *)entity;
  js.push_back(e->amodel);
  js.push_back(e->index);
}
}
namespace Engine {
void MeshDrawer::Serialize(json &js, const Engine::Entity *entity) {
  Drawer::StaticType()->Serialize(js, entity);
  const MeshDrawer *e = (const MeshDrawer *)entity;
  js.push_back(e->mesh);
  js.push_back(e->material);
}
}
namespace Engine {
void AModel::Serialize(json &js, const Engine::Entity *entity) {
  Asset::StaticType()->Serialize(js, entity);
  const AModel *e = (const AModel *)entity;
  js.push_back(e->path);
}
}
namespace Engine {
void Renderer::Serialize(json &js, const Engine::Entity *entity) {
  Component::StaticType()->Serialize(js, entity);
  const Renderer *e = (const Renderer *)entity;
  js.push_back(e->order);
}
}
namespace Engine {
void AShader::Serialize(json &js, const Engine::Entity *entity) {
  Asset::StaticType()->Serialize(js, entity);
  const AShader *e = (const AShader *)entity;
  js.push_back(e->path);
  js.push_back(e->shaderType);
}
}
namespace Engine {
void ATexture::Serialize(json &js, const Engine::Entity *entity) {
  Asset::StaticType()->Serialize(js, entity);
  const ATexture *e = (const ATexture *)entity;
  js.push_back(e->path);
  js.push_back(e->width);
  js.push_back(e->height);
  js.push_back(e->format);
  js.push_back(e->horizontalWrap);
  js.push_back(e->verticalWrap);
  js.push_back(e->minFilter);
  js.push_back(e->magFilter);
}
}
namespace Engine {
void Group::Serialize(json &js, const Engine::Entity *entity) {
  Entity::StaticType()->Serialize(js, entity);
  const Group *e = (const Group *)entity;
  js.push_back(e->dirty);
  js.push_back(e->gameObjects);
}
}
namespace Engine {
void ProjectSetting::Serialize(json &js, const Engine::Entity *entity) {
  Entity::StaticType()->Serialize(js, entity);
  const ProjectSetting *e = (const ProjectSetting *)entity;
  js.push_back(e->serial);
  js.push_back(e->startSceneIndex);
}
}
namespace Engine {
void SceneSetting::Serialize(json &js, const Engine::Entity *entity) {
  Entity::StaticType()->Serialize(js, entity);
  const SceneSetting *e = (const SceneSetting *)entity;
  js.push_back(e->mainCamera);
}
}
namespace Engine {
void Script::Serialize(json &js, const Engine::Entity *entity) {
  Component::StaticType()->Serialize(js, entity);
  const Script *e = (const Script *)entity;
}
}
namespace Engine {
void Transform::Serialize(json &js, const Engine::Entity *entity) {
  Component::StaticType()->Serialize(js, entity);
  const Transform *e = (const Transform *)entity;
  js.push_back(e->localPosition);
  js.push_back(e->localRotation);
  js.push_back(e->localScale);
  js.push_back(e->localEulerAngles);
  js.push_back(e->parent);
  js.push_back(e->children);
  js.push_back(e->dirty);
  js.push_back(e->localToWorldMatrix);
}
}
namespace Engine {
void Asset::Deserialize(json &js, Engine::Entity *entity) {
  const Asset *e = (const Asset *)entity;
  e->name = js.back().get<std::string>(); js.erase(--js.end());
  e->serial = js.back().get<uint64_t>(); js.erase(--js.end());
  Entity::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Component::Deserialize(json &js, Engine::Entity *entity) {
  const Component *e = (const Component *)entity;
  e->gameObject = js.back().get<GameObject *>(); js.erase(--js.end());
  e->enabled = js.back().get<bool>(); js.erase(--js.end());
  e->localEnabled = js.back().get<bool>(); js.erase(--js.end());
  Entity::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Entity::Deserialize(json &js, Engine::Entity *entity) {
  const Entity *e = (const Entity *)entity;
}
}
namespace Engine {
void GameObject::Deserialize(json &js, Engine::Entity *entity) {
  const GameObject *e = (const GameObject *)entity;
  e->components = js.back().get<std::unordered_set<Component *>>(); js.erase(--js.end());
  e->transform = js.back().get<Transform *>(); js.erase(--js.end());
  e->group = js.back().get<Group *>(); js.erase(--js.end());
  e->name = js.back().get<std::string>(); js.erase(--js.end());
  Entity::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Camera::Deserialize(json &js, Engine::Entity *entity) {
  const Camera *e = (const Camera *)entity;
  e->framebuffer = js.back().get<std::shared_ptr<Framebuffer>>(); js.erase(--js.end());
  e->size = js.back().get<float>(); js.erase(--js.end());
  e->fr = js.back().get<float>(); js.erase(--js.end());
  e->nr = js.back().get<float>(); js.erase(--js.end());
  e->aspectRatio = js.back().get<float>(); js.erase(--js.end());
  e->fovy = js.back().get<float>(); js.erase(--js.end());
  e->orthographic = js.back().get<bool>(); js.erase(--js.end());
  e->normalization = js.back().get<glm::mat4>(); js.erase(--js.end());
  e->dirty = js.back().get<bool>(); js.erase(--js.end());
  Renderer::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Drawer::Deserialize(json &js, Engine::Entity *entity) {
  const Drawer *e = (const Drawer *)entity;
  e->order = js.back().get<unsigned>(); js.erase(--js.end());
  Component::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void AFramebuffer::Deserialize(json &js, Engine::Entity *entity) {
  const AFramebuffer *e = (const AFramebuffer *)entity;
  e->depthTexture = js.back().get<ATexture *>(); js.erase(--js.end());
  e->colorTexture = js.back().get<ATexture *>(); js.erase(--js.end());
  Asset::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void AMaterial::Deserialize(json &js, Engine::Entity *entity) {
  const AMaterial *e = (const AMaterial *)entity;
  e->mainTexture = js.back().get<ATexture *>(); js.erase(--js.end());
  e->fragmentShader = js.back().get<AShader *>(); js.erase(--js.end());
  e->vertexShader = js.back().get<AShader *>(); js.erase(--js.end());
  Asset::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void AMesh::Deserialize(json &js, Engine::Entity *entity) {
  const AMesh *e = (const AMesh *)entity;
  e->index = js.back().get<uint32_t>(); js.erase(--js.end());
  e->amodel = js.back().get<AModel *>(); js.erase(--js.end());
  Asset::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void MeshDrawer::Deserialize(json &js, Engine::Entity *entity) {
  const MeshDrawer *e = (const MeshDrawer *)entity;
  e->material = js.back().get<std::shared_ptr<Material>>(); js.erase(--js.end());
  e->mesh = js.back().get<std::shared_ptr<Mesh>>(); js.erase(--js.end());
  Drawer::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void AModel::Deserialize(json &js, Engine::Entity *entity) {
  const AModel *e = (const AModel *)entity;
  e->path = js.back().get<std::string>(); js.erase(--js.end());
  Asset::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Renderer::Deserialize(json &js, Engine::Entity *entity) {
  const Renderer *e = (const Renderer *)entity;
  e->order = js.back().get<unsigned>(); js.erase(--js.end());
  Component::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void AShader::Deserialize(json &js, Engine::Entity *entity) {
  const AShader *e = (const AShader *)entity;
  e->shaderType = js.back().get<unsigned>(); js.erase(--js.end());
  e->path = js.back().get<std::string>(); js.erase(--js.end());
  Asset::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void ATexture::Deserialize(json &js, Engine::Entity *entity) {
  const ATexture *e = (const ATexture *)entity;
  e->magFilter = js.back().get<int>(); js.erase(--js.end());
  e->minFilter = js.back().get<int>(); js.erase(--js.end());
  e->verticalWrap = js.back().get<int>(); js.erase(--js.end());
  e->horizontalWrap = js.back().get<int>(); js.erase(--js.end());
  e->format = js.back().get<unsigned>(); js.erase(--js.end());
  e->height = js.back().get<int>(); js.erase(--js.end());
  e->width = js.back().get<int>(); js.erase(--js.end());
  e->path = js.back().get<std::string>(); js.erase(--js.end());
  Asset::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Group::Deserialize(json &js, Engine::Entity *entity) {
  const Group *e = (const Group *)entity;
  e->gameObjects = js.back().get<std::unordered_set<GameObject *>>(); js.erase(--js.end());
  e->dirty = js.back().get<bool>(); js.erase(--js.end());
  Entity::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void ProjectSetting::Deserialize(json &js, Engine::Entity *entity) {
  const ProjectSetting *e = (const ProjectSetting *)entity;
  e->startSceneIndex = js.back().get<int>(); js.erase(--js.end());
  e->serial = js.back().get<uint64_t>(); js.erase(--js.end());
  Entity::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void SceneSetting::Deserialize(json &js, Engine::Entity *entity) {
  const SceneSetting *e = (const SceneSetting *)entity;
  e->mainCamera = js.back().get<Camera *>(); js.erase(--js.end());
  Entity::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Script::Deserialize(json &js, Engine::Entity *entity) {
  const Script *e = (const Script *)entity;
  Component::StaticType()->Deserialize(js, entity);
}
}
namespace Engine {
void Transform::Deserialize(json &js, Engine::Entity *entity) {
  const Transform *e = (const Transform *)entity;
  e->localToWorldMatrix = js.back().get<glm::mat4>(); js.erase(--js.end());
  e->dirty = js.back().get<bool>(); js.erase(--js.end());
  e->children = js.back().get<std::unordered_set<Transform *>>(); js.erase(--js.end());
  e->parent = js.back().get<Transform *>(); js.erase(--js.end());
  e->localEulerAngles = js.back().get<glm::vec3>(); js.erase(--js.end());
  e->localScale = js.back().get<glm::vec3>(); js.erase(--js.end());
  e->localRotation = js.back().get<glm::quat>(); js.erase(--js.end());
  e->localPosition = js.back().get<glm::vec3>(); js.erase(--js.end());
  Component::StaticType()->Deserialize(js, entity);
}
}
void type_init() {
{
using namespace Engine;
  Asset::StaticType(new Engine::Type("Asset", Engine::instantiate<Asset, true>, Asset::Serialize, Asset::Deserialize));
}
{
using namespace Engine;
  Component::StaticType(new Engine::Type("Component", Engine::instantiate<Component, true>, Component::Serialize, Component::Deserialize));
}
{
using namespace Engine;
  Entity::StaticType(new Engine::Type("Entity", Engine::instantiate<Entity, true>, Entity::Serialize, Entity::Deserialize));
}
{
using namespace Engine;
  GameObject::StaticType(new Engine::Type("GameObject", Engine::instantiate<GameObject, true>, GameObject::Serialize, GameObject::Deserialize));
}
{
using namespace Engine;
  Camera::StaticType(new Engine::Type("Camera", Engine::instantiate<Camera, true>, Camera::Serialize, Camera::Deserialize));
}
{
using namespace Engine;
  Drawer::StaticType(new Engine::Type("Drawer", Engine::instantiate<Drawer, true>, Drawer::Serialize, Drawer::Deserialize));
}
{
using namespace Engine;
  AFramebuffer::StaticType(new Engine::Type("AFramebuffer", Engine::instantiate<AFramebuffer, true>, AFramebuffer::Serialize, AFramebuffer::Deserialize));
}
{
using namespace Engine;
  AMaterial::StaticType(new Engine::Type("AMaterial", Engine::instantiate<AMaterial, true>, AMaterial::Serialize, AMaterial::Deserialize));
}
{
using namespace Engine;
  AMesh::StaticType(new Engine::Type("AMesh", Engine::instantiate<AMesh, true>, AMesh::Serialize, AMesh::Deserialize));
}
{
using namespace Engine;
  MeshDrawer::StaticType(new Engine::Type("MeshDrawer", Engine::instantiate<MeshDrawer, true>, MeshDrawer::Serialize, MeshDrawer::Deserialize));
}
{
using namespace Engine;
  AModel::StaticType(new Engine::Type("AModel", Engine::instantiate<AModel, true>, AModel::Serialize, AModel::Deserialize));
}
{
using namespace Engine;
  Renderer::StaticType(new Engine::Type("Renderer", Engine::instantiate<Renderer, true>, Renderer::Serialize, Renderer::Deserialize));
}
{
using namespace Engine;
  AShader::StaticType(new Engine::Type("AShader", Engine::instantiate<AShader, true>, AShader::Serialize, AShader::Deserialize));
}
{
using namespace Engine;
  ATexture::StaticType(new Engine::Type("ATexture", Engine::instantiate<ATexture, true>, ATexture::Serialize, ATexture::Deserialize));
}
{
using namespace Engine;
  Group::StaticType(new Engine::Type("Group", Engine::instantiate<Group, true>, Group::Serialize, Group::Deserialize));
}
{
using namespace Engine;
  ProjectSetting::StaticType(new Engine::Type("ProjectSetting", Engine::instantiate<ProjectSetting, true>, ProjectSetting::Serialize, ProjectSetting::Deserialize));
}
{
using namespace Engine;
  SceneSetting::StaticType(new Engine::Type("SceneSetting", Engine::instantiate<SceneSetting, true>, SceneSetting::Serialize, SceneSetting::Deserialize));
}
{
using namespace Engine;
  Script::StaticType(new Engine::Type("Script", Engine::instantiate<Script, true>, Script::Serialize, Script::Deserialize));
}
{
using namespace Engine;
  Transform::StaticType(new Engine::Type("Transform", Engine::instantiate<Transform, true>, Transform::Serialize, Transform::Deserialize));
}
}
void type_clear() {
{
using namespace Engine;
  delete Asset::StaticType();
}
{
using namespace Engine;
  delete Component::StaticType();
}
{
using namespace Engine;
  delete Entity::StaticType();
}
{
using namespace Engine;
  delete GameObject::StaticType();
}
{
using namespace Engine;
  delete Camera::StaticType();
}
{
using namespace Engine;
  delete Drawer::StaticType();
}
{
using namespace Engine;
  delete AFramebuffer::StaticType();
}
{
using namespace Engine;
  delete AMaterial::StaticType();
}
{
using namespace Engine;
  delete AMesh::StaticType();
}
{
using namespace Engine;
  delete MeshDrawer::StaticType();
}
{
using namespace Engine;
  delete AModel::StaticType();
}
{
using namespace Engine;
  delete Renderer::StaticType();
}
{
using namespace Engine;
  delete AShader::StaticType();
}
{
using namespace Engine;
  delete ATexture::StaticType();
}
{
using namespace Engine;
  delete Group::StaticType();
}
{
using namespace Engine;
  delete ProjectSetting::StaticType();
}
{
using namespace Engine;
  delete SceneSetting::StaticType();
}
{
using namespace Engine;
  delete Script::StaticType();
}
{
using namespace Engine;
  delete Transform::StaticType();
}
}
