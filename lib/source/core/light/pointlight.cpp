#include "light/pointlight.h"

#include "everywhere.h"
#include "mesh/mesh.h"
#include "material/lightmaterial.h"
#include "misc/util.h"

#include <algorithm>
#include <cmath>


namespace {

static const std::vector<Vertex> VERTICES {
    { { 0.000000, -0.100000, -0.000000 }, { -0.0000, -1.0000, 0.0000 }, { 0.0, 0.0 } },
    { { 0.000000, -0.086603, -0.050000 }, { -0.0000, -0.8438, -0.5367 }, { 0.0, 0.0 } },
    { { 0.029389, -0.086603, -0.040451 }, { 0.3155, -0.8438, -0.4342 }, { 0.0, 0.0 } },
    { { 0.000000, -0.050000, -0.086603 }, { -0.0000, 0.0000, -1.0000 }, { 0.0, 0.0 } },
    { { 0.000000, 0.000000, -0.100000 }, { 0.5159, -0.4791, -0.7101 }, { 0.0, 0.0 } },
    { { 0.058779, 0.000000, -0.080902 }, { 0.0000, -0.4791, -0.8778 }, { 0.0, 0.0 } },
    { { 0.050904, -0.050000, -0.070063 }, { 0.0000, 0.8438, -0.5367 }, { 0.0, 0.0 } },
    { { 0.000000, 0.050000, -0.086603 }, { 0.5159, 0.4791, -0.7101 }, { 0.0, 0.0 } },
    { { 0.000000, 0.086603, -0.050000 }, { -0.0000, 0.4791, -0.8778 }, { 0.0, 0.0 } },
    { { 0.029389, 0.086603, -0.040451 }, { 0.5878, -0.0000, -0.8090 }, { 0.0, 0.0 } },
    { { 0.050904, 0.050000, -0.070063 }, { 0.0000, 1.0000, 0.0000 }, { 0.0, 0.0 } },
    { { 0.000000, 0.100000, 0.000000 }, { 0.3155, 0.8438, -0.4342 }, { 0.0, 0.0 } },
    { { 0.047553, 0.086603, -0.015451 }, { 0.8348, 0.4791, -0.2712 }, { 0.0, 0.0 } },
    { { 0.082364, 0.050000, -0.026762 }, { 0.5104, -0.8438, -0.1659 }, { 0.0, 0.0 } },
    { { 0.082364, -0.050000, -0.026762 }, { 0.9511, 0.0000, -0.3090 }, { 0.0, 0.0 } },
    { { 0.047553, -0.086603, -0.015451 }, { 0.5104, 0.8438, -0.1659 }, { 0.0, 0.0 } },
    { { 0.095106, 0.000000, -0.030902 }, { 0.8348, -0.4791, -0.2712 }, { 0.0, 0.0 } },
    { { 0.082364, -0.050000, 0.026762 }, { 0.5104, -0.8438, 0.1659 }, { 0.0, 0.0 } },
    { { 0.047553, -0.086603, 0.015451 }, { 0.9511, -0.0000, 0.3090 }, { 0.0, 0.0 } },
    { { 0.082364, 0.050000, 0.026762 }, { 0.5104, 0.8438, 0.1659 }, { 0.0, 0.0 } },
    { { 0.095106, -0.000000, 0.030902 }, { 0.8348, -0.4791, 0.2712 }, { 0.0, 0.0 } },
    { { 0.047553, 0.086603, 0.015451 }, { 0.8348, 0.4791, 0.2712 }, { 0.0, 0.0 } },
    { { 0.050904, -0.050000, 0.070063 }, { 0.3155, -0.8438, 0.4342 }, { 0.0, 0.0 } },
    { { 0.029389, -0.086603, 0.040451 }, { 0.5878, 0.0000, 0.8090 }, { 0.0, 0.0 } },
    { { 0.050904, 0.050000, 0.070063 }, { 0.3155, 0.8438, 0.4342 }, { 0.0, 0.0 } },
    { { 0.058779, -0.000000, 0.080902 }, { 0.5159, -0.4791, 0.7101 }, { 0.0, 0.0 } },
    { { 0.029389, 0.086603, 0.040451 }, { 0.5159, 0.4791, 0.7101 }, { 0.0, 0.0 } },
    { { 0.000000, -0.050000, 0.086603 }, { 0.0000, -0.8438, 0.5367 }, { 0.0, 0.0 } },
    { { 0.000000, -0.086603, 0.050000 }, { -0.0000, -0.0000, 1.0000 }, { 0.0, 0.0 } },
    { { 0.000000, 0.050000, 0.086603 }, { -0.0000, 0.8438, 0.5367 }, { 0.0, 0.0 } },
    { { 0.000000, -0.000000, 0.100000 }, { -0.0000, -0.4791, 0.8778 }, { 0.0, 0.0 } },
    { { 0.000000, 0.086603, 0.050000 }, { 0.0000, 0.4791, 0.8778 }, { 0.0, 0.0 } },
    { { -0.050904, -0.050000, 0.070063 }, { -0.3155, -0.8438, 0.4342 }, { 0.0, 0.0 } },
    { { -0.029389, -0.086603, 0.040451 }, { -0.5878, -0.0000, 0.8090 }, { 0.0, 0.0 } },
    { { -0.050904, 0.050000, 0.070063 }, { -0.3155, 0.8438, 0.4342 }, { 0.0, 0.0 } },
    { { -0.058779, -0.000000, 0.080902 }, { -0.5159, -0.4791, 0.7101 }, { 0.0, 0.0 } },
    { { -0.029389, 0.086603, 0.040451 }, { -0.5159, 0.4791, 0.7101 }, { 0.0, 0.0 } },
    { { -0.082364, 0.050000, 0.026762 }, { -0.9511, 0.0000, 0.3090 }, { 0.0, 0.0 } },
    { { -0.095106, -0.000000, 0.030902 }, { -0.5104, 0.8438, 0.1659 }, { 0.0, 0.0 } },
    { { -0.047553, 0.086603, 0.015451 }, { -0.5104, -0.8438, 0.1659 }, { 0.0, 0.0 } },
    { { -0.047553, -0.086603, 0.015451 }, { -0.8348, -0.4791, 0.2712 }, { 0.0, 0.0 } },
    { { -0.082364, -0.050000, 0.026762 }, { -0.8348, 0.4791, 0.2712 }, { 0.0, 0.0 } },
    { { -0.047553, 0.086603, -0.015451 }, { -0.5104, 0.8438, -0.1659 }, { 0.0, 0.0 } },
    { { -0.047553, -0.086603, -0.015451 }, { -0.5104, -0.8438, -0.1659 }, { 0.0, 0.0 } },
    { { -0.095106, 0.000000, -0.030902 }, { -0.8348, -0.4791, -0.2712 }, { 0.0, 0.0 } },
    { { -0.082364, -0.050000, -0.026762 }, { -0.8348, 0.4791, -0.2712 }, { 0.0, 0.0 } },
    { { -0.082364, 0.050000, -0.026762 }, { -0.9511, -0.0000, -0.3090 }, { 0.0, 0.0 } },
    { { -0.029389, -0.086603, -0.040451 }, { -0.3155, -0.8438, -0.4342 }, { 0.0, 0.0 } },
    { { -0.058779, 0.000000, -0.080902 }, { -0.5159, -0.4791, -0.7101 }, { 0.0, 0.0 } },
    { { -0.050904, -0.050000, -0.070063 }, { -0.5159, 0.4791, -0.7101 }, { 0.0, 0.0 } },
    { { -0.029389, 0.086603, -0.040451 }, { -0.5878, 0.0000, -0.8090 }, { 0.0, 0.0 } },
    { { -0.050904, 0.050000, -0.070063 }, { -0.3155, 0.8438, -0.4342 }, { 0.0, 0.0 } },

};

static const std::vector<GLuint> INDICES {
    0, 1, 2, 4, 6, 3,
    8, 10, 7, 3, 2, 1,
    7, 5, 4, 8, 11, 9,
    9, 13, 10, 6, 15, 2,
    10, 16, 5, 9, 11, 12,
    0, 2, 15, 5, 14, 6,
    14, 18, 15, 13, 20, 16,
    12, 11, 21, 0, 15, 18,
    16, 17, 14, 12, 19, 13,
    17, 23, 18, 19, 25, 20,
    21, 11, 26, 0, 18, 23,
    20, 22, 17, 21, 24, 19,
    22, 28, 23, 24, 30, 25,
    26, 11, 31, 0, 23, 28,
    25, 27, 22, 26, 29, 24,
    27, 33, 28, 29, 35, 30,
    31, 11, 36, 0, 28, 33,
    30, 32, 27, 31, 34, 29,
    34, 38, 35, 36, 11, 39,
    0, 33, 40, 35, 41, 32,
    36, 37, 34, 32, 40, 33,
    39, 11, 42, 0, 40, 43,
    38, 45, 41, 39, 46, 37,
    41, 43, 40, 37, 44, 38,
    0, 43, 47, 44, 49, 45,
    42, 51, 46, 45, 47, 43,
    46, 48, 44, 42, 11, 50,
    0, 47, 1, 48, 3, 49,
    50, 7, 51, 49, 1, 47,
    51, 4, 48, 50, 11, 8,
    4, 5, 6, 8, 9, 10,
    3, 6, 2, 7, 10, 5,
    9, 12, 13, 6, 14, 15,
    10, 13, 16, 5, 16, 14,
    14, 17, 18, 13, 19, 20,
    16, 20, 17, 12, 21, 19,
    17, 22, 23, 19, 24, 25,
    20, 25, 22, 21, 26, 24,
    22, 27, 28, 24, 29, 30,
    25, 30, 27, 26, 31, 29,
    27, 32, 33, 29, 34, 35,
    30, 35, 32, 31, 36, 34,
    34, 37, 38, 35, 38, 41,
    36, 39, 37, 32, 41, 40,
    38, 44, 45, 39, 42, 46,
    41, 45, 43, 37, 46, 44,
    44, 48, 49, 42, 50, 51,
    45, 49, 47, 46, 51, 48,
    48, 4, 3, 50, 8, 7,
    49, 3, 1, 51, 7, 4
};

Mesh* CreateSphere(const Color& color) {
    std::shared_ptr<LightMaterial> tempMaterial { new LightMaterial { color } };

    Everywhere::Instance().Get<MaterialStorage>().GetMaterials().Add(tempMaterial);
    size_t materialId =
        Everywhere::Instance().Get<MaterialStorage>().GetLastMaterialID();

    Mesh* mesh = new Mesh(::VERTICES, ::INDICES);
    mesh->SetDrawingMode(MeshDrawingMode::POINTS);
    mesh->SetMaterialId(materialId);

    return mesh;
}

static constexpr float DEFAULT_RADIUS { 1.0f };
static constexpr float DEFAULT_CONSTANT { 1.0f };
static constexpr float DEFAULT_LINEAR_INFLUENCE { 2.0f };
static constexpr float DEFAULT_QUADRATIC_INFLUENCE { 1.0f };

static constexpr float DEFAULT_AMBIENT { 0.2f };
static constexpr float DEFAULT_DEFFUSE { 0.5f };
static constexpr float DEFAULT_SPECULAR { 1.0f };

} // namespace


PointLight::PointLight() :
    PointLight { DEFAULT_AMBIENT, DEFAULT_DEFFUSE, DEFAULT_SPECULAR } {}

PointLight::PointLight(const Color& color) :
    PointLight { color, DEFAULT_AMBIENT, DEFAULT_DEFFUSE, DEFAULT_SPECULAR } {}

PointLight::PointLight(float radius) :
    PointLight { radius, DEFAULT_AMBIENT, DEFAULT_DEFFUSE, DEFAULT_SPECULAR } {}

PointLight::PointLight(const Color& color, float radius) :
    PointLight { color, radius,
                 DEFAULT_AMBIENT, DEFAULT_DEFFUSE, DEFAULT_SPECULAR } {}

PointLight::PointLight(float ambient, float diffuse, float specular) :
    PointLight { Color::WHITE, ambient, diffuse, specular } {}

PointLight::PointLight(float radius,
                       float ambient, float diffuse, float specular) :
    PointLight { Color::WHITE, radius, ambient, diffuse, specular } {}

PointLight::PointLight(const Color& color,
                       float ambient, float diffuse, float specular) :
    PointLight { color, DEFAULT_RADIUS, ambient, diffuse, specular } {}

PointLight::PointLight(const Color& color, float radius,
                       float ambient, float diffuse, float specular) :
    Light { color },
    m_radius { radius },
    m_ambient { ambient }, m_diffuse { diffuse }, m_specular { specular },
    m_constant { DEFAULT_CONSTANT }, m_linear {}, m_quadratic {},
    m_linearInfluence { DEFAULT_LINEAR_INFLUENCE },
    m_quadraticInfluence { DEFAULT_QUADRATIC_INFLUENCE } {
    UpdateCLQByRadius();
    m_childMesh.reset(::CreateSphere(m_color));

    Everywhere::Instance().Get<LightStorage>().GetPointLights().Add(this);
}

PointLight::~PointLight() {
    Everywhere::Instance().Get<LightStorage>().GetPointLights().Delete(this);
}

float PointLight::GetRadius() const {
    return m_radius;
}

float PointLight::GetAmbient() const {
    return m_ambient;
}

float PointLight::GetDiffuse() const {
    return m_diffuse;
}

float PointLight::GetSpecular() const {
    return m_specular;
}

float PointLight::GetConstant() const {
    return m_constant;
}

float PointLight::GetLinear() const {
    return m_linear;
}

float PointLight::GetQuadratic() const {
    return m_quadratic;
}

float PointLight::GetLinearInfluence() const {
    return m_linearInfluence;
}

float PointLight::GetQuadraticInfluence() const {
    return m_quadraticInfluence;
}

Color PointLight::GetAmbientColor() const {
    return Color { static_cast<glm::vec3>(m_color) * m_ambient };
}

Color PointLight::GetDiffuseColor() const {
    return Color { static_cast<glm::vec3>(m_color) * m_diffuse };
}

Color PointLight::GetSpecularColor() const {
    return Color { static_cast<glm::vec3>(m_color) * m_specular };
}

void PointLight::SetRadius(float radius) {
    if (util::IsEqual(radius, 0.0f)) {
        m_radius = radius;
    } else {
        m_radius = std::numeric_limits<decltype(radius)>::min();
    }

    UpdateCLQByRadius();
}

void PointLight::SetAmbient(float ambient) {
    m_ambient = ambient;
}

void PointLight::SetDiffuse(float diffuse) {
    m_diffuse = diffuse;
}

void PointLight::SetSpecular(float specular) {
    m_specular = specular;
}

void PointLight::SetConstant(float constant) {
    m_constant = constant;
}

void PointLight::SetLinear(float linear) {
    m_linear = linear;
}

void PointLight::SetQuadratic(float quadratic) {
    m_quadratic = quadratic;
}

void PointLight::SetLinearInfluence(float linearInfluence) {
    m_linearInfluence = linearInfluence;
}

void PointLight::SetQuadraticInfluence(float quadraticInfluence) {
    m_quadraticInfluence = quadraticInfluence;
}

void PointLight::SetADS(float ambient, float diffuse, float specular) {
    SetAmbient(ambient);
    SetDiffuse(diffuse);
    SetSpecular(specular);
}

void PointLight::SetCLQ(float constant, float linear, float quadratic) {
    SetConstant(constant);
    SetLinear(linear);
    SetQuadratic(quadratic);
}

void PointLight::UpdateCLQByRadius() {
    SetCLQ(GetConstant(),
           GetLinearInfluence() / GetRadius(),
           GetQuadraticInfluence() / std::pow(GetRadius(), 2.0f));
}
