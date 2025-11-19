
import sys
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QLineEdit,
    QPushButton, QTabWidget, QFormLayout, QSpinBox, QDoubleSpinBox, QMessageBox
)
from PyQt5.QtCore import Qt
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

# ====== Finance Logic Converted from C++ ======
class Person:
    def __init__(self):
        self.name = ""
        self.age = 0
        self.salary = 0.0
        self.expense = 0.0
        self.savings = 0.0
        self.invest = 0.0
        self.tax = 0.0
        self.inflation = 0.0
        self.sh = self.eh = self.svh = self.ivh = 0.0
        self.hike_set = False

    def set_data(self, name, age, salary, expense, savings, invest):
        self.name = name
        self.age = age
        self.salary = salary
        self.expense = expense
        self.savings = savings
        self.invest = invest

    def set_rates(self, tax, inflation):
        self.tax = tax
        self.inflation = inflation

    def set_hike(self, sh, eh, svh, ivh):
        self.sh, self.eh, self.svh, self.ivh = sh, eh, svh, ivh
        if svh > sh * 2 or ivh > sh * 2:
            self.hike_set = False
            return False
        self.hike_set = True
        return True

    def check(self):
        if self.expense > self.salary:
            return f"⚠ Expenses exceed salary by ₹{self.expense - self.salary:.2f}"
        elif self.salary - self.expense < self.savings:
            return "⚠ Savings too high for leftover balance."
        elif self.salary - self.expense - self.savings < self.invest:
            return "⚠ Investments too high for available balance."
        else:
            return "✓ Your financial condition looks healthy."

    def proj_simple(self, months):
        if not self.hike_set:
            return None
        mi = self.inflation / 1200.0
        salf = self.salary * (1 + self.sh / 100.0) ** months
        salnet = salf * (1 - self.tax / 100.0)
        savf = self.savings * (1 + self.svh / 100.0) ** months
        invf = self.invest * (1 + self.ivh / 100.0) ** months
        inflf = (1 + mi) ** months
        rs = savf / inflf
        ri = invf / inflf
        return {
            "Gross Salary": salf,
            "Net Salary": salnet,
            "Savings (Nominal)": savf,
            "Savings (Real)": rs,
            "Investments (Nominal)": invf,
            "Investments (Real)": ri
        }

    def proj_detailed(self, months):
        if not self.hike_set:
            return None
        cursal = self.salary
        totsav = 0
        totinv = 0
        for _ in range(months):
            cursal *= (1 + self.sh / 100.0)
            msav = cursal * (self.savings / self.salary)
            minv = cursal * (self.invest / self.salary)
            totsav += msav
            totinv += minv
        mi = self.inflation / 1200.0
        inflf = (1 + mi) ** months
        rs = totsav / inflf
        ri = totinv / inflf
        return {
            "Nominal Savings": totsav,
            "Nominal Investments": totinv,
            "Real Savings": rs,
            "Real Investments": ri,
            "Total Real": rs + ri
        }

# ====== PyQt GUI ======
class FinanceApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Personal Finance Assistant")
        self.setGeometry(100, 100, 800, 600)
        self.person = Person()
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()
        self.tabs = QTabWidget()
        layout.addWidget(self.tabs)

        # --- Tab: Data Entry ---
        self.tab_data = QWidget()
        self.tabs.addTab(self.tab_data, "Data Entry")
        form_layout = QFormLayout()
        self.tab_data.setLayout(form_layout)

        self.name_input = QLineEdit()
        self.age_input = QSpinBox(); self.age_input.setRange(0, 120)
        self.salary_input = QDoubleSpinBox(); self.salary_input.setRange(0, 1e9); self.salary_input.setPrefix("₹")
        self.expense_input = QDoubleSpinBox(); self.expense_input.setRange(0, 1e9); self.expense_input.setPrefix("₹")
        self.savings_input = QDoubleSpinBox(); self.savings_input.setRange(0, 1e9); self.savings_input.setPrefix("₹")
        self.invest_input = QDoubleSpinBox(); self.invest_input.setRange(0, 1e9); self.invest_input.setPrefix("₹")

        form_layout.addRow("Name:", self.name_input)
        form_layout.addRow("Age:", self.age_input)
        form_layout.addRow("Salary:", self.salary_input)
        form_layout.addRow("Expense:", self.expense_input)
        form_layout.addRow("Savings:", self.savings_input)
        form_layout.addRow("Investment:", self.invest_input)

        self.save_data_btn = QPushButton("Save Data")
        self.save_data_btn.clicked.connect(self.save_data)
        form_layout.addRow(self.save_data_btn)

        # --- Tab: Settings ---
        self.tab_settings = QWidget()
        self.tabs.addTab(self.tab_settings, "Settings")
        settings_layout = QFormLayout()
        self.tab_settings.setLayout(settings_layout)

        self.tax_input = QDoubleSpinBox(); self.tax_input.setRange(0, 100); self.tax_input.setSuffix(" %")
        self.infl_input = QDoubleSpinBox(); self.infl_input.setRange(0, 100); self.infl_input.setSuffix(" %")
        settings_layout.addRow("Tax Rate:", self.tax_input)
        settings_layout.addRow("Inflation Rate:", self.infl_input)

        self.sh_input = QDoubleSpinBox(); self.sh_input.setRange(0, 100); self.sh_input.setSuffix(" %")
        self.eh_input = QDoubleSpinBox(); self.eh_input.setRange(0, 100); self.eh_input.setSuffix(" %")
        self.svh_input = QDoubleSpinBox(); self.svh_input.setRange(0, 100); self.svh_input.setSuffix(" %")
        self.ivh_input = QDoubleSpinBox(); self.ivh_input.setRange(0, 100); self.ivh_input.setSuffix(" %")

        settings_layout.addRow("Salary Hike %:", self.sh_input)
        settings_layout.addRow("Expense Hike %:", self.eh_input)
        settings_layout.addRow("Savings Hike %:", self.svh_input)
        settings_layout.addRow("Investment Hike %:", self.ivh_input)

        self.save_settings_btn = QPushButton("Save Settings")
        self.save_settings_btn.clicked.connect(self.save_settings)
        settings_layout.addRow(self.save_settings_btn)

        # --- Tab: Check & Projections ---
        self.tab_proj = QWidget()
        self.tabs.addTab(self.tab_proj, "Projections")
        proj_layout = QVBoxLayout()
        self.tab_proj.setLayout(proj_layout)

        self.check_btn = QPushButton("Check Financial Health")
        self.check_btn.clicked.connect(self.check_health)
        proj_layout.addWidget(self.check_btn)

        self.months_input = QSpinBox(); self.months_input.setRange(1, 120)
        self.months_input.setPrefix("Months: ")
        proj_layout.addWidget(self.months_input)

        self.simple_proj_btn = QPushButton("Simple Projection")
        self.simple_proj_btn.clicked.connect(self.simple_projection)
        proj_layout.addWidget(self.simple_proj_btn)

        self.detailed_proj_btn = QPushButton("Detailed Projection")
        self.detailed_proj_btn.clicked.connect(self.detailed_projection)
        proj_layout.addWidget(self.detailed_proj_btn)

        self.canvas = FigureCanvas(Figure(figsize=(5, 3)))
        proj_layout.addWidget(self.canvas)
        self.ax = self.canvas.figure.subplots()

        self.setLayout(layout)

    # --- Button Functions ---
    def save_data(self):
        self.person.set_data(
            self.name_input.text(),
            self.age_input.value(),
            self.salary_input.value(),
            self.expense_input.value(),
            self.savings_input.value(),
            self.invest_input.value()
        )
        QMessageBox.information(self, "Saved", "Personal data saved successfully!")

    def save_settings(self):
        self.person.set_rates(self.tax_input.value(), self.infl_input.value())
        if not self.person.set_hike(self.sh_input.value(), self.eh_input.value(),
                                    self.svh_input.value(), self.ivh_input.value()):
            QMessageBox.warning(self, "Invalid Hike", "Hikes too high!")
        else:
            QMessageBox.information(self, "Saved", "Settings saved successfully!")

    def check_health(self):
        msg = self.person.check()
        QMessageBox.information(self, "Financial Health", msg)

    def simple_projection(self):
        months = self.months_input.value()
        result = self.person.proj_simple(months)
        if not result:
            QMessageBox.warning(self, "Error", "Set hikes first!")
            return
        self.ax.clear()
        self.ax.bar(result.keys(), result.values(), color='skyblue')
        self.ax.set_title(f"Simple Projection ({months} months)")
        self.ax.tick_params(axis='x', rotation=45)
        self.canvas.draw()

    def detailed_projection(self):
        months = self.months_input.value()
        result = self.person.proj_detailed(months)
        if not result:
            QMessageBox.warning(self, "Error", "Set hikes first!")
            return
        self.ax.clear()
        self.ax.bar(result.keys(), result.values(), color='orange')
        self.ax.set_title(f"Detailed Projection ({months} months)")
        self.ax.tick_params(axis='x', rotation=45)
        self.canvas.draw()

# ====== Run Application ======
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = FinanceApp()
    window.show()
    sys.exit(app.exec_())
