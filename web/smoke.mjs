import { chromium } from "playwright";

const url = process.argv[2] ?? "http://127.0.0.1:8080/";
const channel = process.env.PLAYWRIGHT_CHANNEL || undefined;
const browser = await chromium.launch({
  ...(channel === undefined ? {} : { channel }),
  headless: true,
  args: ["--autoplay-policy=no-user-gesture-required"],
});
const page = await browser.newPage();
page.on("console", (message) => console.log("page:", message.text()));
page.on("pageerror", (error) => console.error("pageerror:", error.message));

try {
  await page.goto(url, { waitUntil: "load" });
  await page.click("#start");
  await page.waitForFunction(
    () => window.__p1 && (window.__p1.ready || window.__p1.error),
    { timeout: 20000 },
  );
  const ready = await page.evaluate(() => window.__p1);
  console.log("ready:", ready);
  if (!ready.ready) throw new Error(ready.error || "engine did not start");
  if (ready.paramCount !== 846)
    throw new Error(`param count ${ready.paramCount}`);

  const box = await page.locator("#note").boundingBox();
  await page.mouse.move(box.x + box.width / 2, box.y + box.height / 2);
  await page.mouse.down();
  await page.waitForTimeout(800);
  const peak = await page.evaluate(() => window.__p1.lastScopeMax);
  await page.mouse.up();
  console.log("scope peak:", peak);
  if (!(peak > 0.0001)) throw new Error("scope stayed silent");

  await page.locator("#level").evaluate((element) => {
    element.value = "42";
    element.dispatchEvent(new Event("input", { bubbles: true }));
  });
  await page.waitForFunction(() => window.__p1.lastParam === 42, {
    timeout: 5000,
  });
  console.log(
    "parameter round trip:",
    await page.evaluate(() => window.__p1.lastParam),
  );
  console.log("web smoke: passed");
} finally {
  await browser.close();
}
