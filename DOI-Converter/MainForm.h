#pragma once

namespace DOIConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Text::RegularExpressions;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace System::IO;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	private: static bool error = false;
	private: static bool processed = false;
	private: static bool folder_opened = false;
	private: static bool canceled_by_user = false;
	private: static Dictionary<String^, String^>^ extensions;
	private: static List<String^>^ files;
	private: static array<String^>^ args;
	private: static String^ title;
	private: static String^ lbl_text;
	private: static String^ file_name;
	private: static String^ btn_text;
	private: static String^ destination_folder;
	private: static String^ ini_file;
	private: static String^ csv_columns;
	private: static String^ current_line;
	private: System::Drawing::Font^ large_font;
	private: System::Drawing::Font^ small_font;
	private: System::Drawing::Font^ medium_font;
	private: System::Timers::Timer^ timer;
	private: delegate void delegate_reset();

	public:
		MainForm(void)
		{
			InitializeComponent();

			this->csv_columns = "Mat. IPTU;Insc. RGI;CPF Transmitente;CPF Adquirente;Data do Registro;Mat. valida?";

			this->large_font = gcnew System::Drawing::Font(L"Segoe UI Light", 90);
			this->medium_font = gcnew System::Drawing::Font(L"Segoe UI Light", 32);
			this->small_font = gcnew System::Drawing::Font(L"Segoe UI Light", 18);
			this->extensions = gcnew Dictionary<String^, String^>;
			this->files = gcnew List<String^>();

			this->extensions->Add(".txt", "Documentos de texto");
			this->extensions->Add(".*", "Todos os arquivos");

			this->lbl_text = L"Clique ou arraste os arquivos";
			this->btn_text = L"Converter DOI -> CSV";
			this->title = L"Conversor DOI v2.0";
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
	private: System::Windows::Forms::Label^ labelStatus;
	private: System::Windows::Forms::ProgressBar^ progressBar;

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
			this->labelDropable->Click += gcnew System::EventHandler(this, &MainForm::labelDropable_Click);
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
			// 
			// progressBar
			// 
			this->progressBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
			this->progressBar->Location = System::Drawing::Point(22, 122);
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
			this->Controls->Add(this->labelDropable);
			this->Controls->Add(this->labelStatus);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->Text = L"Conversor DOI v2.0";
			this->ResumeLayout(false);

		}
#pragma endregion

	public:
		void OnDragEnter(DragEventArgs^ e) override
		{
			if (timer) timer->Stop();

			e->Effect = DragDropEffects::None;
			labelStatus->ForeColor = System::Drawing::Color::FromArgb(62, 80, 98);

			if (e->Data->GetDataPresent(DataFormats::FileDrop))
			{
				auto list = safe_cast<array<String^>^>(e->Data->GetData(DataFormats::FileDrop));

				MainForm::BackColor = System::Drawing::Color::LightCoral;
				labelStatus->ForeColor = System::Drawing::Color::White;
				labelStatus->Text = L"Formato inválido!";
				labelStatus->Font = medium_font;

				labelDropable->Visible = false;
				files->Clear();


				if (FilesToList(list)->Length)
				{
					labelStatus->Text = L"ↆ";
					labelStatus->Font = large_font;
					labelStatus->ForeColor = System::Drawing::Color::FromArgb(62, 80, 98);
					MainForm::BackColor = System::Drawing::Color::FromArgb(238, 238, 238);

					e->Effect = DragDropEffects::Copy;
				}
			}
		}
		
	public:
		void OnDragLeave(EventArgs^ e) override
		{
			labelDropable->Visible = true;
			labelStatus->Text = lbl_text;
			labelStatus->Font = medium_font;
			MainForm::BackColor = System::Drawing::Color::FromArgb(238, 238, 238);
		}

	public:
		void OnDragDrop(DragEventArgs^ e) override
		{
			labelStatus->Text = "Processando...";
			labelStatus->Font = medium_font;

			auto list = files->ToArray();

			Cursor = Cursors::WaitCursor;

			for (int i = 0; i < list->Length; i++)
			{
				if (canceled_by_user)
					break;

				Convert(list[i], 1 + i);
			}

			Clear();
		}

	private:
		System::Void labelDropable_Click(System::Object^ sender, System::EventArgs^ e)
		{
			labelDropable->Text = "Selecionando arquivos...";

			auto open_file_dialog = gcnew OpenFileDialog;

			String^ filter = "";

			for each (KeyValuePair<String^, String^> kvp in extensions)
				filter += kvp.Value + " (*" + kvp.Key + ")|*" + kvp.Key + "|";

			open_file_dialog->Filter = filter->Substring(0, filter->Length - 1);
			open_file_dialog->RestoreDirectory = true;
			open_file_dialog->Multiselect = true;

			if (open_file_dialog->ShowDialog() == Windows::Forms::DialogResult::OK)
			{
				files->Clear();

				labelDropable->Visible = false;
				labelDropable->Text = lbl_text;
				labelDropable->Font = medium_font;

				auto list = FilesToList(open_file_dialog->FileNames);

				Cursor = Cursors::WaitCursor;

				for (int i = 0; i < list->Length; i++)
				{
					if (canceled_by_user)
						break;

					Convert(list[i], 1 + i);
				}
			}
			else
				canceled_by_user = true;

			labelDropable->Text = lbl_text;

			Clear();
		}

	private:
		void Clear()
		{
			this->progressBar->Visible = false;
			labelDropable->Visible = false;

			if (canceled_by_user)
			{
				MainForm::BackColor = System::Drawing::Color::LightCoral;
				labelStatus->ForeColor = System::Drawing::Color::White;
				labelStatus->Text = "Cancelado pelo usuário!";
				canceled_by_user = false;
				WaitNSeconds(2);
			}
			else
			{
				MainForm::BackColor = System::Drawing::Color::LightSeaGreen;
				labelStatus->ForeColor = System::Drawing::Color::White;
				labelStatus->Text = "Concluído!";
				labelStatus->Font = medium_font;
				progressBar->Value = 0;

				if (error)
				{
					error = false;
					labelStatus->Text = "Concluído com erro!";
				}

				if (!processed)
				{
					labelStatus->Text = "Erro no processamento!";
					MainForm::BackColor = System::Drawing::Color::LightCoral;
				}

				WaitNSeconds(2);
			}

			Cursor = Cursors::Default;
			folder_opened = false;
			processed = false;
			files->Clear();
		}

	private:
		void ElapsedReset(System::Object^ sender, System::Timers::ElapsedEventArgs^ e)
		{
			if (InvokeRequired)
				BeginInvoke(gcnew delegate_reset(this, &MainForm::Reset));
		}

	private:
		void Reset()
		{
			if (InvokeRequired)
				BeginInvoke(gcnew delegate_reset(this, &MainForm::Reset));
			else {
				labelDropable->Visible = true;
				labelStatus->Text = "Processando...";
				labelStatus->ForeColor = System::Drawing::Color::FromArgb(62, 80, 98);
				MainForm::BackColor = System::Drawing::Color::FromArgb(238, 238, 238);
			}
		}

	private:
		static array<String^>^ FilesToList(array<String^>^ list)
		{
			files->Clear();

			for (int i = 0; i < list->Length; i++)
			{
				auto file = list[i];
				auto ext = Path::GetExtension(file);

				if (extensions->ContainsKey(ext) && !Directory::Exists(file) && !IsBinary(file))
					files->Add(file);
			}

			return files->ToArray();
		}

	private:
		static bool IsBinary(String^ file_path)
		{
			constexpr int chars_to_check = 8000;

			int nul_count = 0;

			auto stream_reader = gcnew StreamReader(file_path);

			for (int i = 0; i < chars_to_check; i++)
			{
				constexpr char nul_char = '\0';

				if (stream_reader->EndOfStream)
					return false;

				if (static_cast<char>(stream_reader->Read()) == nul_char)
				{
					nul_count++;

					if (nul_count >= 1)
						return true;
				}
				else
				{
					nul_count = 0;
				}
			}

			return false;
		}

	private:
		void WaitNSeconds(const int seconds)
		{
			timer = gcnew System::Timers::Timer();
			timer->Interval = seconds * 1000;
			timer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &MainForm::ElapsedReset);
			timer->AutoReset = false;
			timer->Enabled = true;
		}

	private:
		bool ValidInscription(String^ full_inscription)
		{
			full_inscription = full_inscription->Trim()->Replace("-", "");

			if (String::IsNullOrEmpty(full_inscription) || full_inscription->Length < 3 || full_inscription->Length > 8) return false;

			const auto inscription_number = Int32::Parse(full_inscription->Substring(0, full_inscription->Length - 1));
			auto digit = Int32::Parse(full_inscription->Substring(full_inscription->Length - 1, 1));
			auto inscription = String::Format("{0:000000}", inscription_number);
			auto result_array = gcnew List<int>();
			float summation = 0;

			for (int i = 0; i < inscription->Length; i++)
			{
				auto new_value = float::Parse(inscription->Substring(i, 1));

				if (i % 2 == 1)
					new_value = (new_value * 2) / 10;

				auto values = new_value.ToString()->Split(',');

				result_array->Add(Int32::Parse(values[0]));

				if (values->Length > 1)
					result_array->Add(Int32::Parse(values[1]));
			}

			for (int i = 0; i < result_array->Count; i++)
				summation += result_array[i];

			const int final_digit = (Math::Ceiling(summation / 10) * 10) - summation;

			return digit.Equals(final_digit);
		}

	private:
		String^ NumbersOnly(String^ number)
		{
			Regex^ rx = gcnew Regex("(\\d*)");
			MatchCollection^ matches = rx->Matches(number);
			String^ numbers = "";

			for each (Match ^ match in matches)
				numbers += match->Value;

			return numbers;
		}

	private:
		void Convert(String^ file, const int current)
		{
			auto sr = gcnew StreamReader(file);
			auto fn = Path::GetFileName(file);
			auto fd = Path::GetDirectoryName(file);
			auto fnx = Path::GetFileNameWithoutExtension(file);
			file_name = Path::GetFileName(fn);

			try
			{
				int length = 0;
				int last_key = 0;
				const int total = files->Count;

				String^ line;

				if (!folder_opened)
				{
					destination_folder = fd;

					if (files->Count > 1)
					{
						auto save_file_dialog = gcnew SaveFileDialog();

						save_file_dialog->FileName = "Selecionar pasta";
						save_file_dialog->InitialDirectory = fd;
						save_file_dialog->Title = "Selecionar pasta";

						if (save_file_dialog->ShowDialog() != Windows::Forms::DialogResult::OK)
						{
							canceled_by_user = true;

							return;
						}

						destination_folder = Path::GetDirectoryName(save_file_dialog->FileName);
					}

					folder_opened = true;
				}

				auto sb = gcnew StringBuilder();
				auto csv_file = destination_folder + "\\" + fnx + ".csv";

				sb->Append(csv_columns);

				int current_line_number = 0;

				bool valid_inscription = false;
				String^ date = "";

				while ((line = sr->ReadLine()))
				{
					current_line = line;

					try
					{
						const int key = Int32::Parse(Get(line, 1, 1));

						current_line_number++;

						if (key == last_key)
							continue;

						if (key == 1)
						{
							auto inscription = Get(line, 320, 15);
							auto rgi = Get(line, 42, 7)->Trim();
							auto inscription_number = NumbersOnly(inscription);

							date = Get(line, 12, 10);

							inscription = Check(current_line_number, 320, inscription);
							rgi = Check(current_line_number, 42, rgi->Trim());

							if (inscription == "ERROR" || "ERROR" == rgi) return;

							sb->Append(Environment::NewLine + inscription_number + ";" + rgi + ";");

							valid_inscription = ValidInscription(inscription_number);
						}
						else if (key == 2)
						{
							auto inscription_transmitter = Get(line, 15, 11);

							inscription_transmitter = Check(current_line_number, 15, inscription_transmitter);

							if (inscription_transmitter == "ERROR") return;

							sb->Append(inscription_transmitter + ";");
						}
						else if (key == 3)
						{
							auto inscription_purchaser = Get(line, 15, 11);

							inscription_purchaser = Check(current_line_number, 15, inscription_purchaser);

							if (inscription_purchaser == "ERROR") return;

							String^ valid_inscription_message = valid_inscription ? "sim" : "nao";

							sb->Append(inscription_purchaser + ";" + date + ";" + valid_inscription_message + ";");
						}

						if (key == 1) length++;

						last_key = key;
					}
					catch (Exception^ ex)
					{
						Error(ex, current_line_number, 0);
						error = true;

						return sr->Close();
					}
				}

				auto converting_text = "Convertendo " + fn + "...\n\n";

				if (total > 1)
					converting_text = converting_text + "arquivo " + current + "/" + total;

				labelStatus->Font = small_font;
				labelStatus->Text = converting_text;

				progressBar->Value = current * 100 / total;
				progressBar->Visible = true;

				try
				{
					auto sw = gcnew StreamWriter(csv_file, false);

					sw->WriteLine(sb->ToString());
					sw->Close();

					processed = true;
				}
				catch (Exception^ ex)
				{
					Error(ex, 0, 0);
					error = true;
				}
			}
			finally
			{
				sr->Close();

				delete sr;
			}
		}
	private:
		static String^ Get(String^ line, const int from, const int length)
		{
			String^ text = "";

			try
			{
				text = line->Substring(from - 1, length);
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(ex->Message + Environment::NewLine + ex->StackTrace, title);
			}

			return text;
		}

	private:
		static String^ Check(const int line_number, int column, String^ text)
		{
			String^ new_text;

			try
			{
				new_text = text->Substring(0, text->Length - 1);
			}
			catch (Exception^ ex)
			{
				new_text = "ERROR";

				Error(ex, line_number, column);
			}

			return new_text;
		}

	private:
		static void Error(Exception^ ex, const int line_number, const int column)
		{
			auto sb = gcnew StringBuilder();
			auto lb = gcnew String(Environment::NewLine);

			String^ text = ex->Message;
			String^ caption = "Erro no processamento!";

			if (line_number > 0 && column > 0)
			{
				sb->Append("Arquivo:\t" + file_name + lb);
				sb->Append("Linha:\t" + line_number + lb);

				if (column < current_line->Length)
					sb->Append("Coluna:\t" + column + lb);

				sb->Append(lb + "Exceção:\n" + ex->Message);

				caption = "Erro no Layout do arquivo!";
				text = sb->ToString();
				error = true;
			}

			caption = title + ": " + caption;

			MessageBox::Show(text, caption, MessageBoxButtons::OK, MessageBoxIcon::Warning);
		}
	};
}
