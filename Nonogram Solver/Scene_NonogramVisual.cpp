#include "Scene_NonogramVisual.h"
#include "framework.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "Collision.h"
#include "ClickableButton.h"
#include "FileDialog.h"
#include "IDialogBox.h"

#include <algorithm>


using namespace std::placeholders;

Scene_NonogramVisual::Scene_NonogramVisual()
{
	SetNonogram(new Nonogram(L"nonograms/15x15_Mous.nono"));

	m_pWriteFactory = DRAW->GetWriteFactory();

	INPUT->BindMouseUpAction(InputKeys::MouseLeft, this, std::bind(&Scene_NonogramVisual::ClickMouse, this, _1, _2));

	for (size_t i{}; i < 6; ++i)
	{
		auto button = new ClickableButton();
		m_Buttons.push_back(button);
		button->SetDimensions(300, 30);
		button->SetPosition({ 150, float(15 + 35 * i) });
		button->SetTextColor({ 0,0,0,1 });
		AddObjectToScene(button);
	}

	m_CancelButton = new ClickableButton();
	m_CancelButton->SetButtonColor({ 0.8f,0.1f,0.1f,1 });
	m_CancelButton->SetTextColor({ 0,0,0,1 });
	m_CancelButton->SetDimensions(100, 30);
	m_CancelButton->SetText(_T("Stop Solving"));
	m_CancelButton->OnClick.BindFunction(this, [this]() {this->GetNonogram()->Unlock(); });
	AddObjectToScene(m_CancelButton);

	m_Buttons[0]->SetText(_T("Save to file"));
	m_Buttons[0]->OnClick.BindFunction(this, [this]()
		{
			if (auto pNonogram = this->GetNonogram()) {
				new InputStringDialog(_T("File name:"));
			InputStringDialog::OnOk.BindFunction([pNonogram](const tstring& string)
				{
					pNonogram->SaveToFile(_T("nonograms/") + string + _T(".nono"));
				});
			}
		});

	m_Buttons[1]->SetText(_T("Open file"));
	m_Buttons[1]->OnClick.BindFunction(this, [this]()
		{
			std::wstring filepath;
			HRESULT hr = BasicFileOpen(filepath, { L"Nonograms *.nono*", L"*.nono*" });
			if (SUCCEEDED(hr)) this->SetNonogram(new Nonogram(filepath));
		});
	
	m_Buttons[2]->SetText(_T("Set Width"));
	m_Buttons[2]->OnClick.BindFunction(this, [this]()
		{
			if (auto pNonogram = this->GetNonogram()) {
				new InputStringDialog(_T("New Width:"));
				InputStringDialog::OnOk.BindFunction([this](const tstring& string)
					{
						try {
							int width = std::stoi(string);
							this->SetNonogram(new Nonogram(width, this->GetNonogram()->GetHeight()));
						}
						catch (std::exception e)
						{

						}
					});
			}
		});

	m_Buttons[3]->SetText(_T("Set Height"));
	m_Buttons[3]->OnClick.BindFunction(this, [this]()
		{
			if (auto pNonogram = this->GetNonogram()) {
				new InputStringDialog(_T("New Height:"));
				InputStringDialog::OnOk.BindFunction([this](const tstring& string)
					{
						try {
							int height = std::stoi(string);
							this->SetNonogram(new Nonogram(this->GetNonogram()->GetWidth(), height));
						}
						catch (std::exception e)
						{

						}
					});
			}
		});

	m_Buttons[4]->SetText(_T("Solve Recursive Backtracking"));
	m_Buttons[4]->OnClick.BindFunction(this, [this]()
		{
			if (auto pNonogram = this->GetNonogram())
				this->StartSolvingThread(std::bind(&Nonogram::SolveRecursiveBacktracking, pNonogram));
		});

	m_Buttons[5]->SetText(_T("Solve Improved Recursive Backtracking"));
	m_Buttons[5]->OnClick.BindFunction(this, [this]()
		{
			if (auto pNonogram = this->GetNonogram())
				this->StartSolvingThread(std::bind(&Nonogram::SolveImprovedRecursiveBacktracking, pNonogram));
		});

	UpdateTextSize(32);
}

Scene_NonogramVisual::~Scene_NonogramVisual()
{
	if (m_pNonogram) m_pNonogram->Unlock();
	if (m_SolvingThread.valid()) m_SolvingThread.wait();

	SafeDelete(m_pNonogram);
	SafeRelease(m_pTextFormat);
}

void Scene_NonogramVisual::Update(float deltaTime)
{
	Scene::Update(deltaTime);

	if (!m_pNonogram) return;

	// calculate grid hitbox in case position has changed
	int horSquares = m_pNonogram->GetWidth() + m_HorizontalHintSize;
	int verSquares = m_pNonogram->GetHeight() + m_VerticalHintSize;

	IVector2 screenSize = ENGINE->GetWindowResolution();
	m_SquareSize = min(float(screenSize.x) / horSquares, float(screenSize.y) / verSquares);

	m_GridOffset.x = (screenSize.x - horSquares * m_SquareSize) / 2;
	m_GridOffset.y = (screenSize.y - verSquares * m_SquareSize) / 2;

	m_GridBounds.left	= m_GridOffset.x + m_HorizontalHintSize * m_SquareSize;
	m_GridBounds.top	= m_GridOffset.y + m_VerticalHintSize * m_SquareSize;
	m_GridBounds.right	= m_GridOffset.x + horSquares * m_SquareSize;
	m_GridBounds.bottom = m_GridOffset.y + verSquares * m_SquareSize;

	auto size = ENGINE->GetClientSize();
	m_CancelButton->SetPosition({ size.x - 50.f, 15.f });

	if (m_pTextFormat->GetFontSize() != float(m_SquareSize)) UpdateTextSize(float(m_SquareSize));
}

void Scene_NonogramVisual::Draw(const FVector2& windowSize) const
{
	
	auto pDraw = DRAW;
	pDraw->SetDrawingColor({ 0,0,0,1 });

	pDraw->ClearBackground({ 1,1,1,1 });
	pDraw->SetOpacity(1.f);

	if (m_pNonogram) {

		int horSquares = m_pNonogram->GetWidth() + m_HorizontalHintSize;
		int verSquares = m_pNonogram->GetHeight() + m_VerticalHintSize;

		// draw outline
		//pDraw->DrawRectangle(FRect{
		//	float(m_GridOffset.x),
		//	float(m_GridOffset.y),
		//	float(m_GridOffset.x + horSquares * m_SquareSize),
		//	float(m_GridOffset.y + verSquares * m_SquareSize) },
		//	{ 1,0,0,1 }, 5.f);
		//pDraw->DrawRectangle(m_GridBounds,{ 1,0,0,1 }, 5.f);

		// draw main grid
		for (int x = 0; x < m_pNonogram->GetWidth(); ++x)
			for (int y = 0; y < m_pNonogram->GetHeight(); ++y)
			{
				FRect drawingRect{
					float(m_GridOffset.x + (x + m_HorizontalHintSize) * m_SquareSize),
					float(m_GridOffset.y + (y + m_VerticalHintSize) * m_SquareSize),
					float(m_GridOffset.x + (x + 1 + m_HorizontalHintSize) * m_SquareSize),
					float(m_GridOffset.y + (y + 1 + m_VerticalHintSize) * m_SquareSize)
				};
				if (m_pNonogram->getGrid()[x + y * m_pNonogram->GetWidth()])
					pDraw->FillRectangle(drawingRect, FColor{ 0.05f,0.05f,0.05f,1 });
				else if (m_pNonogram->getImpossibleGrid()[x + y * m_pNonogram->GetWidth()])
					pDraw->FillRectangle(drawingRect, FColor{ 0.75f,0.75f,0.75f,1 });
				else pDraw->DrawRectangle(drawingRect, FColor{ 0,0,0,1 }, 1.f);
			}

		// draw top hints
		for (int x = 0; x < m_pNonogram->GetWidth(); ++x)
		{
			auto& hints = m_pNonogram->GetVerticalHints()[x];

			auto it = hints.rbegin();
			for (int y = 0; y < m_VerticalHintSize; ++y)
			{
				FRect drawRect{
					float(m_GridOffset.x + (m_HorizontalHintSize + x) * m_SquareSize),
					float(m_GridOffset.y + (m_VerticalHintSize - y - 1) * m_SquareSize),
					float(m_GridOffset.x + (m_HorizontalHintSize + x + 1) * m_SquareSize),
					float(m_GridOffset.y + (m_VerticalHintSize - y) * m_SquareSize) };
				pDraw->DrawRectangle(drawRect);

				if (it != hints.rend())
				{
					pDraw->DrawTextW(drawRect, std::to_wstring(*it), m_pTextFormat);
					++it;
				}
			}
		}

		// draw left hints
		for (int y = 0; y < m_pNonogram->GetHeight(); ++y)
		{
			auto& hints = m_pNonogram->GetHorizontalHints()[y];

			auto it = hints.rbegin();
			for (int x = 0; x < m_HorizontalHintSize; ++x)
			{
				FRect drawRect{
					float(m_GridOffset.x + (m_HorizontalHintSize - x - 1) * m_SquareSize),
					float(m_GridOffset.y + (m_VerticalHintSize + y) * m_SquareSize),
					float(m_GridOffset.x + (m_HorizontalHintSize - x) * m_SquareSize),
					float(m_GridOffset.y + (m_VerticalHintSize + y + 1) * m_SquareSize) };
				pDraw->DrawRectangle(drawRect);

				if (it != hints.rend())
				{
					pDraw->DrawTextW(drawRect, std::to_wstring(*it), m_pTextFormat);
					++it;
				}
			}
		}

		// Draw thick lines every 5 squares
		for (int x = 0; x < m_pNonogram->GetWidth(); x += 5)
			for (int y = 0; y < m_pNonogram->GetHeight(); y += 5)
			{
				FRect drawingRect{
					float(m_GridOffset.x + (x + m_HorizontalHintSize) * m_SquareSize),
					float(m_GridOffset.y + (y + m_VerticalHintSize) * m_SquareSize),
					float(m_GridOffset.x + (x + 5 + m_HorizontalHintSize) * m_SquareSize),
					float(m_GridOffset.y + (y + 5 + m_VerticalHintSize) * m_SquareSize)
				};
				pDraw->DrawRectangle(drawingRect, FColor{ 0,0,0,1 }, 3.f);
			}
	}

	Scene::Draw(windowSize);
}

void Scene_NonogramVisual::SetNonogram(Nonogram* pNonogram)
{
	if (!pNonogram) return;

	SafeDelete(m_pNonogram);
	m_pNonogram = pNonogram;

	UpdateHintSize();
}

void Scene_NonogramVisual::StartSolvingThread(std::function<void()> pSolver)
{
	if (!m_SolvingThread.valid() || m_SolvingThread.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
	{
		m_SolvingThread = std::async(pSolver);
	}
}

void Scene_NonogramVisual::ClickMouse(int x, int y)
{
	if (IsOverlapping(FVector2{ float(x),float(y) }, m_GridBounds))
	{
		float width = m_GridBounds.right - m_GridBounds.left;
		float height = m_GridBounds.bottom - m_GridBounds.top;

		m_pNonogram->SwitchSquare(
			int(float(x - m_GridBounds.left) / width * m_pNonogram->GetWidth()),
			int(float(y - m_GridBounds.top) / height * m_pNonogram->GetHeight()));
		UpdateHintSize();
	}
}

void Scene_NonogramVisual::UpdateTextSize(float size)
{
	SafeRelease(m_pTextFormat);
	HRESULT hr = m_pWriteFactory->CreateTextFormat(
		_T("Verdana"),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
		size,
		_T(""),
		&m_pTextFormat
	);
	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WHOLE_WORD);
}

void Scene_NonogramVisual::UpdateHintSize()
{
	auto verHints = m_pNonogram->GetVerticalHints();
	m_VerticalHintSize = std::max_element(verHints.begin(), verHints.end(), [](const std::vector<int>& v0, const std::vector<int>& v1)
		{
			return v0.size() < v1.size();
		})->size();

	auto horHints = m_pNonogram->GetHorizontalHints();
	m_HorizontalHintSize = std::max_element(horHints.begin(), horHints.end(), [](const std::vector<int>& v0, const std::vector<int>& v1)
		{
			return v0.size() < v1.size();
		})->size();
}
