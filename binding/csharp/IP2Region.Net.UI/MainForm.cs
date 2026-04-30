using IP2Region.Net.XDB;

namespace IP2Region.Net.UI;

public class MainForm : Form
{
    private readonly TextBox _ipInput = new() { PlaceholderText = "請輸入 IP，例如 8.8.8.8", Dock = DockStyle.Top };
    private readonly Button _searchButton = new() { Text = "查詢城市", Dock = DockStyle.Top, Height = 36 };
    private readonly Label _resultLabel = new() { Text = "查詢結果會顯示在這裡", Dock = DockStyle.Fill, AutoSize = false, TextAlign = ContentAlignment.MiddleCenter };

    private readonly Searcher _searcher;

    public MainForm()
    {
        Text = "IP 城市查詢";
        Width = 520;
        Height = 220;
        StartPosition = FormStartPosition.CenterScreen;

        var dbPath = Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "data", "ip2region_v4.xdb"));
        _searcher = new Searcher(CachePolicy.File, dbPath);

        var panel = new Panel { Dock = DockStyle.Fill, Padding = new Padding(16) };
        panel.Controls.Add(_resultLabel);
        panel.Controls.Add(_searchButton);
        panel.Controls.Add(_ipInput);

        _searchButton.Click += OnSearchClick;

        Controls.Add(panel);
    }

    private void OnSearchClick(object? sender, EventArgs e)
    {
        var ip = _ipInput.Text.Trim();
        if (string.IsNullOrWhiteSpace(ip))
        {
            _resultLabel.Text = "請先輸入 IP";
            return;
        }

        try
        {
            var region = _searcher.Search(ip);
            var city = region.Split('|').FirstOrDefault(part => part != "0" && !string.IsNullOrWhiteSpace(part)) ?? "未知城市";
            _resultLabel.Text = $"IP: {ip}\n城市: {city}\n完整資料: {region}";
        }
        catch (Exception ex)
        {
            _resultLabel.Text = $"查詢失敗：{ex.Message}";
        }
    }

    protected override void Dispose(bool disposing)
    {
        if (disposing)
        {
            _searcher.Dispose();
        }

        base.Dispose(disposing);
    }
}
