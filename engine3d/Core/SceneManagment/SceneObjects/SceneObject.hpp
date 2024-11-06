#pragma once

/*
 * SceneObject
 * @param id - the ID of the SceneObject itself
 * @note actual wrapper for SceneObject for ECS
 * */
#include <Core/ApplicationManager/GameObjManager/UUID.hpp>
#include <Core/ApplicationManager/Scene.hpp>

namespace engine3d
{
  class SceneObject 
  {
    public:
      SceneObject() = default;
      SceneObject(entt::entity handle, Scene *scene);
      SceneObject(const SceneObject &) = default;
      SceneObject(Scene* scene);

      template <typename T, typename... Args> void AddComponent(Args &&...args) 
      {
        printf("Working0\n");
        //! @note when adding component
        T &component = m_ParentScene->m_SceneRegistry.emplace<T>(
            SceneObjectHandler, std::forward<Args>(args)...);
        printf("Working1\n");
        // Adding refrence to component
        component.SetSceneObjectRef(*(this));
        printf("Working2\n");
        //! @note Call to component to do specific action when integrating with
        //! gameObject
        component.OnIntegrate();
      }

      template <typename T> T &SceneGetComponent() 
      {
        // Checking if SceneObject contains this component.
        if (!this->HasComponent<T>())
        {
          coreLogError("SceneObject does not have component!");
          return NULL;
        }

        return m_ParentScene->m_SceneRegistry.get<T>(SceneObjectHandler);
      }

      // Checking if SceneObject has a component
      template <typename T> bool HasComponent() 
      {
        // has<T> has been renamed to all_of<T>
        return m_ParentScene->m_SceneRegistry.all_of<T>(SceneObjectHandler);
      }

      template <typename T> void RemoveComponent() 
      {
        if (!this->HasComponent<T>())
          {
            coreLogError("SceneObject does not have component!");
          }
        m_ParentScene->m_SceneRegistry.remove<T>(SceneObjectHandler);
      }

      operator bool() const { return SceneObjectHandler != entt::null; }

      operator entt::entity() const { return SceneObjectHandler; }

      operator uint32_t() const { return (uint32_t)SceneObjectHandler; }

      bool operator==(const SceneObject &other) const 
      {
        return SceneObjectHandler == other.SceneObjectHandler and
               m_ParentScene == other.m_ParentScene;
      }

      bool operator!=(const SceneObject &other) const { return !(*this == other); }

      UUID GetUUID() { return objectID; }

    private:
      entt::entity SceneObjectHandler{entt::null};
      UUID objectID;
      Scene *m_ParentScene = nullptr; // 12 bytes
  };
}; // namespace Engine3D
