#pragma once

namespace HerrDerBloecke {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace SlimDX;
    using namespace SlimDX::Direct3D9;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            fViewport = gcnew Viewport(100, 100, 100, 100);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    // F**KING C++/CLI GENERATED BULLSHIT FIX IT PLZ
    private: Viewport^ fViewport;
    protected: 

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
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
            this->SuspendLayout();
            // 
            // pictureBox1
            // 
            this->pictureBox1->Location = System::Drawing::Point(300, 246);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(100, 50);
            this->pictureBox1->TabIndex = 0;
            this->pictureBox1->TabStop = false;
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(645, 396);
            this->Controls->Add(this->pictureBox1);
            this->Name = L"MainWindow";
            this->Text = L"MainWindow";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
	};
}
