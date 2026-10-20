#!/usr/bin/env python3
"""Génère le dashboard Grafana « Serre connectée » (JSON classique, provisionné).

Usage : python3 build_dashboard.py > serre-connectee.json
Déploiement sur greenhouse : copier le JSON dans /var/lib/grafana/dashboards/serre/
(provisionné par /etc/grafana/provisioning/dashboards/serre.yaml, relu toutes les 60 s).
"""
import json

DS = {"type": "influxdb", "uid": "P8DA813DF0E63FA04"}
UPC = "#8a1438"  # bordeaux Université Paris Cité
# Carte affichée, codée en dur : les dashboards publics de Grafana refusent les variables de template.
HOST, HOST_LABEL = "arduino7", "Serre 1"  # cf. Server/arduino_db.txt

_pid = 0


def pid():
    global _pid
    _pid += 1
    return _pid


def q_series(measurement, typ, alias, ref="A", agg="mean"):
    return {
        "datasource": DS, "refId": ref, "alias": alias, "rawQuery": True, "resultFormat": "time_series",
        "query": f'SELECT {agg}("value") FROM "{measurement}" WHERE "host" = \'{HOST}\' AND "type" = \'{typ}\' '
                 f'AND $timeFilter GROUP BY time($__interval) fill(null)',
    }


def q_last(measurement, typ, ref="A", window="15m"):
    # état courant : dernière valeur des 15 dernières minutes, indépendamment de la plage affichée
    return {
        "datasource": DS, "refId": ref, "rawQuery": True, "resultFormat": "time_series",
        "query": f'SELECT last("value") FROM "{measurement}" WHERE "host" = \'{HOST}\' AND "type" = \'{typ}\' '
                 f'AND time > now() - {window}',
    }


def text(content, gp, title="", transparent=True):
    return {"type": "text", "id": pid(), "title": title, "transparent": transparent, "gridPos": gp,
            "options": {"mode": "html", "content": content}}




def stat(title, targets, gp, unit="none", decimals=None, mappings=None, thresholds=None, desc="",
         color_mode="background", text_mode="value", fields="", calcs=("lastNotNull",), graph="none"):
    defaults = {"unit": unit, "mappings": mappings or [], "color": {"mode": "thresholds"},
                "thresholds": {"mode": "absolute", "steps": thresholds or [{"color": "text", "value": None}]},
                "noValue": "Pas de données"}
    if decimals is not None:
        defaults["decimals"] = decimals
    return {"type": "stat", "id": pid(), "title": title, "description": desc, "datasource": DS,
            "gridPos": gp, "targets": targets,
            "fieldConfig": {"defaults": defaults, "overrides": []},
            "options": {"reduceOptions": {"calcs": list(calcs), "fields": fields, "values": False},
                        "colorMode": color_mode, "graphMode": graph, "justifyMode": "center",
                        "textMode": text_mode, "orientation": "auto", "wideLayout": True,
                        "showPercentChange": False}}


def actuator(title, typ, gp, on_color, desc):
    m = [{"type": "value", "options": {
        "0": {"text": "Arrêt", "color": "#4a4d55", "index": 0},
        "1": {"text": "Marche", "color": on_color, "index": 1}}}]
    return stat(title, [q_last("switches_value", typ)], gp, mappings=m, desc=desc)


def timeseries(title, targets, gp, unit, desc="", colors=None, minv=None, maxv=None, decimals=1,
               extra_overrides=None, fill=12):
    overrides = [{"matcher": {"id": "byName", "options": a},
                  "properties": [{"id": "color", "value": {"mode": "fixed", "fixedColor": c}}]}
                 for a, c in (colors or {}).items()]
    overrides += extra_overrides or []
    defaults = {"unit": unit, "decimals": decimals, "color": {"mode": "palette-classic"},
                "custom": {"drawStyle": "line", "lineInterpolation": "smooth", "lineWidth": 2,
                           "fillOpacity": fill, "gradientMode": "opacity", "showPoints": "never",
                           "spanNulls": 300000, "axisBorderShow": False, "axisPlacement": "auto"},
                "noValue": "Pas de données"}
    if minv is not None:
        defaults["min"] = minv
    if maxv is not None:
        defaults["max"] = maxv
    return {"type": "timeseries", "id": pid(), "title": title, "description": desc, "datasource": DS,
            "gridPos": gp, "targets": targets,
            "fieldConfig": {"defaults": defaults, "overrides": overrides},
            "options": {"legend": {"displayMode": "table", "placement": "bottom", "showLegend": True,
                                   "calcs": ["lastNotNull", "min", "mean", "max"]},
                        "tooltip": {"mode": "multi", "sort": "none"}}}


def current(title, typ, gp, unit, color, decimals=1, desc=""):
    return stat(title, [q_last("sensors_value", typ)], gp, unit=unit, decimals=decimals, desc=desc,
                thresholds=[{"color": color, "value": None}], color_mode="value", graph="none")


panels = []

# Disposition pour un écran 1920×1080 en mode kiosque : 26 unités de hauteur au total,
# aucune ligne repliable, tout visible sans défilement.


def compact(p, title_size=13, value_size=None):
    p["options"]["text"] = {"titleSize": title_size}
    if value_size:
        p["options"]["text"]["valueSize"] = value_size
    return p


def legend_list(p):
    p["options"]["legend"] = {"displayMode": "list", "placement": "bottom", "showLegend": True, "calcs": []}
    return p


# --- En-tête (h=4) ---------------------------------------------------------------
panels.append(text(
    '<div style="height:100%;display:flex;align-items:center;justify-content:center;background:#ffffff;'
    'border-radius:6px;padding:4px 10px">'
    '<img src="/assets/logo-upc.svg" alt="Université Paris Cité" style="max-height:88px;max-width:100%"></div>',
    {"h": 4, "w": 4, "x": 0, "y": 0}))
panels.append(text(
    f'<div style="height:100%;display:flex;flex-direction:column;justify-content:center;'
    f'border-left:4px solid {UPC};padding-left:16px">'
    f'<div style="font-size:28px;font-weight:600;line-height:1.2">Serre connectée — {HOST_LABEL}</div>'
    f'<div style="opacity:.75;margin-top:6px">Supervision en temps réel · carte Arduino <code>{HOST}</code>'
    '</div></div>',
    {"h": 4, "w": 6, "x": 4, "y": 0}))
panels.append(compact(stat(
    "Mode de pilotage", [q_last("switches_value", "switch_auto")], {"h": 4, "w": 3, "x": 10, "y": 0},
    mappings=[{"type": "value", "options": {
        "0": {"text": "Manuel", "color": "orange", "index": 0},
        "1": {"text": "Automatique", "color": "blue", "index": 1}}}],
    desc="Position du commutateur AUTO de la serre."), value_size=26))
panels.append(compact(stat(
    "Dernière mesure",
    [{"datasource": DS, "refId": "A", "rawQuery": True, "resultFormat": "time_series",
      "query": f'SELECT last("value") FROM "status_value" WHERE "host" = \'{HOST}\' '
               'AND "type" = \'live_time\' AND time > now() - 30d'}],
    {"h": 4, "w": 3, "x": 13, "y": 0}, unit="dateTimeFromNow", fields="/^Time$/",
    thresholds=[{"color": "text", "value": None}], color_mode="value"), value_size=26))
panels.append(compact(stat(
    "Arduino en marche depuis", [q_last("status_value", "live_time")], {"h": 4, "w": 3, "x": 16, "y": 0},
    unit="dtdurations", color_mode="value"), value_size=26))
panels.append(compact(stat(
    "Version firmware", [q_last("status_value", "soft_version")], {"h": 4, "w": 3, "x": 19, "y": 0},
    unit="none", decimals=0, color_mode="value"), value_size=26))

panels.append(text(
    '<div style="height:100%;display:flex;flex-direction:column;align-items:center;justify-content:center;'
    'background:#ffffff;border-radius:6px;padding:4px">'
    '<img src="/assets/qr-github.svg" alt="QR code du dépôt GitHub" style="flex:1;min-height:0;max-width:100%">'
    '<div style="color:#24292f;font-size:11px;font-weight:600;line-height:1.3">Projet sur GitHub</div></div>',
    {"h": 4, "w": 2, "x": 22, "y": 0}))

# --- Actionneurs (h=3 + h=5) -------------------------------------------------------
acts = [
    ("Chauffage", "switch_heat", "red", "Résistance chauffante du sol."),
    ("Ventilation", "switch_fan", "blue", "Ventilateur d'extraction."),
    ("Éclairage", "switch_light", "yellow", "Éclairage horticole."),
    ("Alarme", "switch_alarm", "red", "Alarme générale : active dès qu'un seuil d'alarme est franchi."),
]
for i, (t, typ, c, d) in enumerate(acts):
    panels.append(compact(actuator(t, typ, {"h": 3, "w": 6, "x": 6 * i, "y": 4}, c, d),
                          title_size=14, value_size=24))
tl_series = acts + [("Mode automatique", "switch_auto", "blue", "")]
panels.append({
    "type": "state-timeline", "id": pid(), "title": "Historique des actionneurs (marche / arrêt)", "datasource": DS,
    "gridPos": {"h": 5, "w": 24, "x": 0, "y": 7},
    "targets": [q_series("switches_value", typ, t, ref=chr(65 + i), agg="max")
                for i, (t, typ, _, _) in enumerate(tl_series)],
    "fieldConfig": {"defaults": {
        "mappings": [{"type": "value", "options": {
            "0": {"text": "Arrêt", "color": "#33363d", "index": 0},
            "1": {"text": "Marche", "color": "green", "index": 1}}}],
        "color": {"mode": "thresholds"},
        "thresholds": {"mode": "absolute", "steps": [{"color": "green", "value": None}]},
        "custom": {"fillOpacity": 85, "lineWidth": 0, "spanNulls": 300000}},
        "overrides": [{"matcher": {"id": "byName", "options": t},
                       "properties": [{"id": "mappings", "value": [{"type": "value", "options": {
                           "0": {"text": "Arrêt", "color": "#33363d", "index": 0},
                           "1": {"text": "Marche", "color": c, "index": 1}}}]}]}
                      for t, _, c, _ in tl_series]},
    "options": {"mergeValues": True, "showValue": "never", "alignValue": "left", "rowHeight": 0.85,
                "legend": {"showLegend": False}, "tooltip": {"mode": "single"}}})


# --- Températures et humidités (h=7) : courbe w=9 + deux valeurs courantes w=3 -------
def pair(title, unit, series, x, minv=None, maxv=None, desc=""):
    y = 12
    targets = [q_series("sensors_value", typ, label, ref=chr(65 + i)) for i, (label, typ, _) in enumerate(series)]
    panels.append(legend_list(timeseries(title, targets, {"h": 7, "w": 9, "x": x, "y": y}, unit, desc=desc,
                                         colors={l: c for l, _, c in series}, minv=minv, maxv=maxv)))
    for i, (label, typ, c) in enumerate(series):
        h = 4 if i == 0 else 3
        panels.append(compact(current(label, typ, {"h": h, "w": 3, "x": x + 9, "y": y + 4 * i}, unit, c),
                              value_size=30))


pair("Températures (°C)", "celsius", [
    ("Air intérieur", "int_air_temp", "#ff9830"),
    ("Sol (thermocouple)", "ground_temp", UPC),
], x=0, desc="Température de l'air (DHT) et du sol (thermocouple, référence de la régulation du chauffage).")
pair("Humidité relative (%)", "percent", [
    ("Air intérieur", "int_air_hr", "#5ac4dc"),
    ("Sol", "ground_hr", "#96d98d"),
], x=12, minv=0, maxv=100,
    desc="Humidité relative de l'air (DHT) et du sol (sonde capacitive, 0 % sec – 100 % immergé).")


# --- Lumière, CO2, flux (h=7) : valeur courante + courbe miniature -----------------
def spark(title, typ, x, unit, color, decimals=1, desc=""):
    p = stat(title, [q_series("sensors_value", typ, title)], {"h": 7, "w": 8, "x": x, "y": 19}, unit=unit,
             decimals=decimals, desc=desc, thresholds=[{"color": color, "value": None}], color_mode="value",
             graph="area")
    panels.append(compact(p, title_size=14, value_size=40))


spark("Luminosité relative (%)", "ambiant_lux", 0, "percent", "#fade2a",
      desc="Luminosité ambiante relative (photorésistance, % de la pleine échelle).")
spark("Concentration en CO₂ (ppm)", "co2_ppm", 8, "ppm", "#b877d9", decimals=0,
      desc="Capteur MH-Z à sortie PWM. Une valeur négative indique un capteur absent ou en défaut.")
spark("Flux thermique au sol (W/m²)", "energy_flux", 16, "Wm2", "#f2495c",
      desc="Flux de chaleur mesuré par la sonde fluxmétrique.")

dashboard = {
    "uid": "serre-connectee",
    "title": "Serre connectée — Supervision",
    "description": "État de la serre : actionneurs, températures, humidités, lumière, CO₂, flux thermique. Disposition une page pour écran kiosque.",
    "tags": ["serre", "arduino", "upc"],
    "timezone": "browser",
    "editable": True,
    "graphTooltip": 1,
    "refresh": "1m",
    "time": {"from": "now-3d", "to": "now"},
    "timepicker": {"refresh_intervals": ["1m", "5m", "15m", "1h"]},
    "schemaVersion": 41,
    "fiscalYearStartMonth": 0,
    "links": [],
    "templating": {"list": []},
    "annotations": {"list": [{"builtIn": 1, "datasource": {"type": "grafana", "uid": "-- Grafana --"},
                              "enable": True, "hide": True, "iconColor": "rgba(0, 211, 255, 1)",
                              "name": "Annotations & Alerts", "type": "dashboard"}]},
    "panels": panels,
}

if __name__ == "__main__":
    print(json.dumps(dashboard, ensure_ascii=False, indent=2))
