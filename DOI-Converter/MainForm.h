#pragma once

namespace DOIConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ labelDropable;
	protected:

	protected:

	private: System::Windows::Forms::Label^ labelStatus;
	private: System::Windows::Forms::ProgressBar^ progressBar;
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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->labelDropable = (gcnew System::Windows::Forms::Label());
			this->labelStatus = (gcnew System::Windows::Forms::Label());
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->SuspendLayout();
			// 
			// labelDropable
			// 
			this->labelDropable->AutoEllipsis = true;
			this->labelDropable->Cursor = System::Windows::Forms::Cursors::Hand;
			this->labelDropable->Dock = System::Windows::Forms::DockStyle::Fill;
			this->labelDropable->Font = (gcnew System::Drawing::Font(L"Segoe UI Light", 32, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelDropable->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(62)), static_cast<System::Int32>(static_cast<System::Byte>(80)),
				static_cast<System::Int32>(static_cast<System::Byte>(98)));
			this->labelDropable->Location = System::Drawing::Point(0, 0);
			this->labelDropable->Name = L"labelDropable";
			this->labelDropable->Size = System::Drawing::Size(498, 261);
			this->labelDropable->TabIndex = 0;
			this->labelDropable->Text = L"Clique ou arraste os arquivos";
			this->labelDropable->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelStatus
			// 
			this->labelStatus->AutoEllipsis = true;
			this->labelStatus->Dock = System::Windows::Forms::DockStyle::Fill;
			this->labelStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI Light", 32, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelStatus->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(62)), static_cast<System::Int32>(static_cast<System::Byte>(80)),
				static_cast<System::Int32>(static_cast<System::Byte>(98)));
			this->labelStatus->Location = System::Drawing::Point(0, 0);
			this->labelStatus->Name = L"labelStatus";
			this->labelStatus->Size = System::Drawing::Size(498, 261);
			this->labelStatus->TabIndex = 0;
			this->labelStatus->Text = L"Processando...";
			this->labelStatus->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->labelStatus->Visible = false;
			// 
			// progressBar
			// 
			this->progressBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->progressBar->Location = System::Drawing::Point(22, 120);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(452, 23);
			this->progressBar->TabIndex = 1;
			this->progressBar->Visible = false;
			// 
			// MainForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(498, 261);
			this->Controls->Add(this->progressBar);
			this->Controls->Add(this->labelStatus);
			this->Controls->Add(this->labelDropable);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->Text = L"Conversor DOI v2.0";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
