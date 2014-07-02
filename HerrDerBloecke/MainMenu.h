#pragma once

namespace HdB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainMenu
	/// </summary>
    enum class GameType
    {
        kNewGame = 1,
        kCPUGame,
        kLoadGame,
        kExit,
    };

	public ref class MainMenu : public System::Windows::Forms::Form
	{
	public:
		MainMenu(void)
		{
			InitializeComponent();
            Game = GameType::kExit;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainMenu()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Label^  lblTitle;
    private: System::Windows::Forms::Button^  btnNew;
    private: System::Windows::Forms::Button^  btnCPU;
    private: System::Windows::Forms::Button^  btnLoad;

    public: 
        property GameType Game;
    private: System::Windows::Forms::Button^  btnExit;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->lblTitle = (gcnew System::Windows::Forms::Label());
            this->btnNew = (gcnew System::Windows::Forms::Button());
            this->btnCPU = (gcnew System::Windows::Forms::Button());
            this->btnLoad = (gcnew System::Windows::Forms::Button());
            this->btnExit = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // lblTitle
            // 
            this->lblTitle->AutoSize = true;
            this->lblTitle->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->lblTitle->Location = System::Drawing::Point(70, 9);
            this->lblTitle->Name = L"lblTitle";
            this->lblTitle->Size = System::Drawing::Size(274, 55);
            this->lblTitle->TabIndex = 0;
            this->lblTitle->Text = L"Hauptmenü";
            this->lblTitle->TextAlign = System::Drawing::ContentAlignment::TopCenter;
            // 
            // btnNew
            // 
            this->btnNew->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->btnNew->Location = System::Drawing::Point(147, 77);
            this->btnNew->Name = L"btnNew";
            this->btnNew->Size = System::Drawing::Size(120, 45);
            this->btnNew->TabIndex = 1;
            this->btnNew->Text = L"Neues Spiel";
            this->btnNew->UseVisualStyleBackColor = true;
            this->btnNew->Click += gcnew System::EventHandler(this, &MainMenu::btnNew_Click);
            // 
            // btnCPU
            // 
            this->btnCPU->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->btnCPU->Location = System::Drawing::Point(147, 128);
            this->btnCPU->Name = L"btnCPU";
            this->btnCPU->Size = System::Drawing::Size(120, 45);
            this->btnCPU->TabIndex = 2;
            this->btnCPU->Text = L"Neues Spiel gegen CPU";
            this->btnCPU->UseVisualStyleBackColor = true;
            this->btnCPU->Click += gcnew System::EventHandler(this, &MainMenu::btnCPU_Click);
            // 
            // btnLoad
            // 
            this->btnLoad->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->btnLoad->Location = System::Drawing::Point(147, 179);
            this->btnLoad->Name = L"btnLoad";
            this->btnLoad->Size = System::Drawing::Size(120, 45);
            this->btnLoad->TabIndex = 3;
            this->btnLoad->Text = L"Spiel Laden";
            this->btnLoad->UseVisualStyleBackColor = true;
            this->btnLoad->Click += gcnew System::EventHandler(this, &MainMenu::btnLoad_Click);
            // 
            // btnExit
            // 
            this->btnExit->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->btnExit->Location = System::Drawing::Point(147, 230);
            this->btnExit->Name = L"btnExit";
            this->btnExit->Size = System::Drawing::Size(120, 45);
            this->btnExit->TabIndex = 4;
            this->btnExit->Text = L"Spiel verlassen";
            this->btnExit->UseVisualStyleBackColor = true;
            this->btnExit->Click += gcnew System::EventHandler(this, &MainMenu::btnExit_Click);
            // 
            // MainMenu
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(414, 300);
            this->Controls->Add(this->btnExit);
            this->Controls->Add(this->btnLoad);
            this->Controls->Add(this->btnCPU);
            this->Controls->Add(this->btnNew);
            this->Controls->Add(this->lblTitle);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->Name = L"MainMenu";
            this->Text = L"Hauptmenü";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void btnNew_Click(System::Object^  sender, System::EventArgs^  e) {
                 Game = GameType::kNewGame;
             }
private: System::Void btnCPU_Click(System::Object^  sender, System::EventArgs^  e) {
             Game = GameType::kCPUGame;
         }
private: System::Void btnLoad_Click(System::Object^  sender, System::EventArgs^  e) {
             Game = GameType::kLoadGame;
         }
private: System::Void btnExit_Click(System::Object^  sender, System::EventArgs^  e) {
             Game = GameType::kExit;
         }
};
}
