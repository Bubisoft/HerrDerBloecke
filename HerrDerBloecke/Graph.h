#pragma once

namespace HdB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::Collections::Generic;
	/// <summary>
	/// Zusammenfassung für Graph
	/// </summary>
	public ref class Graph : public System::Windows::Forms::Form
	{

    private:
        List<UInt32>^ mPointsOverTime;

    public:
        property List<UInt32>^ PointsOverTime
        {
            void set(List<UInt32>^ _points)
            {
                mPointsOverTime=_points;
            }
        };
	public:
		Graph(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Graph()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
            this->SuspendLayout();
            // 
            // Graph
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(866, 571);
            this->Name = L"Graph";
            this->Text = L"Graph";
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Graph::Graph_Paint);
            this->Resize += gcnew System::EventHandler(this, &Graph::Graph_Resize);
            this->ResumeLayout(false);

        }
#pragma endregion
    private: System::Void Graph_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {

                 e->Graphics->Clear(this->BackColor);
                 Pen^ pen=gcnew Pen(Color::Black);
                 pen->Width=2;
                 UInt32 delta=this->Size.Width / mPointsOverTime->Count;
                 UInt32 max=0;

                 for(int x=0; x< mPointsOverTime->Count;++x) //get the maximum for the scaling
                 {
                     if(mPointsOverTime[x]>max)
                         max=mPointsOverTime[x];
                 }

                 float scale=(float)(this->ClientSize.Height-10) / (float)max;
                 if(scale>1)
                     scale=1; //no scaling required

                 int i=0;
                 Point lastPoint= Point(0,this->ClientSize.Height);
                 for each(UInt32 u in mPointsOverTime)
                 {
                     Point p=Point(i*delta,this->ClientSize.Height-(u*scale));
                     e->Graphics->DrawLine(pen,lastPoint,p);
                     lastPoint=p;
                     ++i;
                 }

                 System::Drawing::Font^ f=gcnew System::Drawing::Font("Arial",10);

                 for(int i=1;i<=10;++i)
                 {
                     e->Graphics->DrawString((max/i).ToString(),f,Brushes::Black,Point(0,((max*scale)/10)*i));
                 }
             }
    private: System::Void Graph_Resize(System::Object^  sender, System::EventArgs^  e) {
                 Invalidate();
             }
    };
}
