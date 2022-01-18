#include "Scene.h"
#include "framework.h"
#include "IObject.h"
#include "Text.h"
#include "FPSDisplay.h"
#include <algorithm>

Scene::Scene()
{
}

Scene::~Scene()
{
	for (IObject* object : m_Objects) 
		delete object;
}

void Scene::Draw(const FVector2& windowSize) const
{
	IBaseClass::Draw(windowSize);

	for (IObject* pObject : m_Objects)
	{
		if (pObject->IsVisible()) pObject->Draw(windowSize);
	}

}

void Scene::FixedUpdate()
{
	IBaseClass::FixedUpdate();

	for (IObject* pObject : m_Objects)
		if (pObject->IsActive()) pObject->FixedUpdate();
}

void Scene::Update(float deltaTime)
{
	IBaseClass::Update(deltaTime);

	auto sortingLambda = [](IObject* p0, IObject* p1) {return p0->GetDrawingOrder() > p1->GetDrawingOrder(); };

	if (!std::is_sorted(m_Objects.begin(), m_Objects.end(), sortingLambda))
	{
		std::sort(m_Objects.begin(), m_Objects.end(), sortingLambda);
	}

	for (IObject* pObject : m_Objects)
		if (pObject->IsActive()) pObject->Update(deltaTime);
}

void Scene::LateUpdate()
{
	IBaseClass::LateUpdate();

	for (IObject* pObject : m_Objects)
		if (pObject->IsActive()) pObject->LateUpdate();
}

void Scene::BeginPlay()
{
	IBaseClass::BeginPlay();
}

void Scene::OnDestroy()
{
	IBaseClass::OnDestroy();
}

void Scene::Destroy()
{
	IBaseClass::Destroy();
}

void Scene::AddObjectToScene(IObject* pObject)
{
	if (pObject) {
		m_Objects.push_back(pObject);
		m_NewlyAddedObjects.push_front(pObject);
	}
}

void Scene::InitializeNewObjects()
{
	for (IObject* pObject : m_NewlyAddedObjects) pObject->BeginPlay();
	m_NewlyAddedObjects.clear();
}

void Scene::DestroyFlaggedObjects()
{
	for (size_t i{m_Objects.size() - 1}; i != size_t(0) - 1; --i)
	{
		if (!m_Objects[i]->GetDestroyFlag())
		{
			// make objects check components
			m_Objects[i]->DestroyFlaggedComponents();
		}
		else
		{
			delete m_Objects[i];
			m_Objects[i] = m_Objects.back();
			m_Objects.pop_back();
		}
	}
}
