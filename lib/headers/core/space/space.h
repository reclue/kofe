#ifndef SPACE_H
#define SPACE_H

#include "interface/iprocess.h"
#include "interface/icanbeeverywhere.h"
#include "interface/icanbematrix.h"
#include "misc/collectionof.h"
#include "scene/scene.h"

#include <glm/glm.hpp>
#include <string>


class Space final :
    public IProcess,
    public ICanBeEverywhere,
    public ICanBeMatrix {
private:
    static const glm::mat4 MODEL;

private:
    CollectionOf<Scene> m_scenes;

public:
    Space(const Space&) = delete;
    Space(Space&&) noexcept = delete;
    Space& operator=(const Space&) = delete;
    Space& operator=(Space&&) noexcept = delete;

    Space();
    ~Space();

public:
    CollectionOf<Scene>& GetScenes();
    const CollectionOf<Scene>& GetScenes() const;

public: /* IProcess */
    void Processing() override;

public: /* ICanBeMatrix */
    glm::mat4 ToMatrix() const override;
};

#endif // SPACE_H
