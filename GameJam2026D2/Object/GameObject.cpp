#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize()
{
}

void GameObject::Update(float delta_second)
{
}

void GameObject::Draw() const
{
    if (image != -1)
    {
        DrawGraph((int)location.x, (int)location.y, image, TRUE);
    }
}

void GameObject::Finalize()
{
}

const Vector2D& GameObject::GetLocation() const
{
    return location;
}

const unsigned char GameObject::GetZLayer() const
{
    return z_layer;
}

const bool GameObject::GetMobility() const
{
    return is_mobility;
}

void GameObject::SetInGamePoint(InGame* point)
{
    Ingame = point;
}


void GameObject::OnHitCollision(GameObject* hit_object)
{
}

void GameObject::OnAreaDetection(GameObject* hit_object)
{
}

void GameObject::NoHit()
{
}

void GameObject::SetLocation(const Vector2D& location)
{
    this->location = location;
}
