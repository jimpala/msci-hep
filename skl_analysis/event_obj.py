

class Event:

    # Static variables.

    processes = ['WZ', 'ZZ', 'WW', 'stopWt', 'stops', 'stopt', 'ttbar',
               'Zl', 'Zcl', 'Zbl', 'Zcc', 'Zbc', 'Zbb', 'Wl', 'Wcl',
               'Wbl', 'Wcc', 'Wbc', 'Wbb', 'qqWlvH125']

    process_general_map = {
        'qqZvvH125': 'VH',
        'qqWlvH125': 'VH',
        'Wbb': 'V+jets',
        'Wbc': 'V+jets',
        'Wcc': 'V+jets',
        'Wbl': 'V+jets',
        'Wcl': 'V+jets',
        'Wl': 'V+jets',
        'Zbb': 'V+jets',
        'Zbc': 'V+jets',
        'Zcc': 'V+jets',
        'Zbl': 'V+jets',
        'Zcl': 'V+jets',
        'Zl': 'V+jets',
        'ttbar': 'ttbar',
        'stopt': 'stop',
        'stops': 'stop',
        'stopWt': 'stop',
        'WW': 'diboson',
        'ZZ': 'diboson',
        'WZ': 'diboson'
    }

    scale_factor_map = {
        2: {
            'Zl': 1.0,
            'Zcl': 1.41,
            'Zcc': 1.15,
            'Zbl': 1.15,
            'Zbc': 1.15,
            'Zbb': 1.15,
            'Wl': 1.00,
            'Wcl': 1.22,
            'Wcc': 1.65,
            'Wbl': 1.65,
            'Wbc': 1.65,
            'Wbb': 1.65,
            'stopWt': 0.92,
            'stopt': 0.92,
            'stops': 0.92,
            'ttbar': 0.80,
            'multijet': 0.63,
            'WW': 1.13,
            'ZZ': 1.13,
            'WZ': 1.13,
            'qqZvvH125': 1.0,
            'qqWlvH125': 1.0
        }, 3: {
            'Zl': 1.0,
            'Zcl': 1.0,
            'Zcc': 1.1,
            'Zbl': 1.1,
            'Zbc': 1.1,
            'Zbb': 1.1,
            'Wl': 1.00,
            'Wcl': 1.27,
            'Wcc': 1.59,
            'Wbl': 1.59,
            'Wbc': 1.59,
            'Wbb': 1.59,
            'stopWt': 0.99,
            'stopt': 0.99,
            'stops': 0.99,
            'ttbar': 0.87,
            'multijet': 0.6,
            'WW': 1.12,
            'ZZ': 1.12,
            'WZ': 1.12,
            'qqZvvH125': 1.0,
            'qqWlvH125': 1.0
        }
    }

    process_color_map =   {'WW': '#333333',
                             'WZ': '#CCCCCC',
                             'Wbb': '#006600',
                             'Wbc': '#007700',
                             'Wbl': '#009900',
                             'Wcc': '#00CC00',
                             'Wcl': '#66CC66',
                             'Wl': '#99FF99',
                             'ZZ': '#999999',
                             'Zbb': '#0066CC',
                             'Zbc': '#0066CC',
                             'Zbl': '#3399FF',
                             'Zcc': '#6699FF',
                             'Zcl': '#6699CC',
                             'Zl': '#99CCFF',
                             'qqWlvH125': '#FF0000',
                             'stopWt': '#FFFF66',
                             'stops': '#CC9900',
                             'stopt': '#CC9900',
                             'ttbar': '#FFCC00'}

    #####################################################
    #####################################################

    # Constructor

    def __init__(self, process, n_jets, index, event_weight, decision_value=None):
        self.process = process
        self.process_general = Event.process_general_map[process]
        self.classification = 1 if self.process_general == 'VH' else 0
        self.n_jets = n_jets
        self.index = index
        self.event_weight = event_weight
        self.scale_factor = Event.scale_factor_map[self.n_jets][self.process]
        self.post_fit_weight = self.event_weight * self.scale_factor

        self.decision_value = decision_value

        self.train_weight = None

    def set_decision_value(self, decision_value):
        self.decision_value = decision_value

    def is_in_bin(self, low_edge, high_edge):
        if low_edge <= self.decision_value < high_edge:
            return self.event_weight * self.scale_factor
        else:
            return 0

    def set_train_weight(self, scale):
        # Store pretrain weight and enter train mode
        self.train_weight = self.event_weight * scale

