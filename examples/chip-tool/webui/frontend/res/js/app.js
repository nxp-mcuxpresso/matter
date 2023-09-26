/*
 *    Copyright (c) 2017, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *    POSSIBILITY OF SUCH DAMAGE.
 */
(function() {
    angular
        .module('StarterApp', ['ngMaterial', 'ngMessages'])
        .controller('AppCtrl', AppCtrl)
        //controller the drawer hide action upon user click
        .directive('closeDrawer', function() {
            return {
                restrict: 'A',
                link: function(scope, element, attrs) {
                    element.bind('click', function() {
                        var drawer = document.querySelector('.mdl-layout__drawer');
                        if(drawer) {
                            var obfuscator = document.querySelector('.mdl-layout__obfuscator');
                            obfuscator.classList.remove('is-visible');
                            drawer.classList.toggle('is-visible');
                        }
                    });
                }
            };
        })
        .service('sharedProperties', function() {
            var index = 0;
            var networkInfo;

            return {
                getIndex: function() {
                    return index;
                },
                setIndex: function(value) {
                    index = value;
                },
            };
        });

    function AppCtrl($scope, $http, $mdDialog, $interval, sharedProperties) {
        $scope.menu = [{
                title: 'Home',
                icon: 'home',
                show: true,
            },
            {
                title: 'Pairing',
                icon: 'add_circle_outline',
                show: false,
                subitems: [
                    {
                        title: 'Onnetwork',
                        icon: 'info_outline',
                        show: false,
                    },
                    {
                        title: 'Ble-Wifi',
                        icon: 'info_outline',
                        show: false,
                    },
                    {
                        title: 'Ble-Thread',
                        icon: 'info_outline',
                        show: false,
                    },
                ]
            },
            {
                title: 'OnOff',
                icon: 'add_circle_outline',
                show: false,
            },
            {
                title: 'Subscribe',
                icon: 'add_circle_outline',
                show: false,
            },
            {
                title: 'Status',
                icon: 'info_outline',
                show: false,
            },
        ];

        $scope.onoff = {
            attr: 'on-off',
            //nodeId: '1111111122222222',
            nodeId: '1',
            endPointId: 1,
            type_on: 'on',
            type_off: 'off',
            type_toggle: 'toggle',
            type_read: 'read',
        };

        $scope.pairing = {
            nodeId: '1',
            pinCode: '20202021',
            ssId: 'xyz',
            password: 'secret',
            discriminator: '3840',
            dataset: '0e080000000000010000000300001335060004001fffe002084fe76e9a8b5edaf50708fde46f999f0698e20510d47f5027a414ffeebaefa92285cc84fa030f4f70656e5468726561642d653439630102e49c0410b92f8c7fbb4f9f3e08492ee3915fbd2f0c0402a0fff8',
            type_onnetwork: 'onnetwork',
            type_ble_wifi: 'ble-wifi',
            type_ble_thread: 'ble-thread',
        };

        $scope.subscribe = {
            subscribe_cluster : "onoff subscribe on-off",
            minInterval: '10',
            maxInterval: '50',
            nodeId: '1',
            endPointId: 1,
        };

        $scope.headerTitle = 'Home';
        $scope.status = [];
        $scope.reports = [];
        $scope.subscribeStatus = [];

        $scope.isLoading = false;

        $scope.showPanels = function(index) {
            $scope.headerTitle = $scope.menu[index].title;
            for (var i = 0; i < 5; i++) {
                /* set i as the number of menu*/
                $scope.menu[i].show = false;
            }

            $scope.menu[index].show = true;
        };

        $scope.showsubPanels = function(index) {
            $scope.headerTitle = "Pairing: " + $scope.menu[1].subitems[index].title;
            for (var i = 0; i < 3; i++) {
                /* set i as the number of menu*/
                $scope.menu[1].subitems[i].show = false;
            }
            $scope.menu[1].subitems[index].show = true;
        };

        $scope.showAlert = function(ev, operation, result) {
            $mdDialog.show(
                $mdDialog.alert()
                .parent(angular.element(document.querySelector('#popupContainer')))
                .clickOutsideToClose(true)
                .title('Information')
                .textContent(operation + ' operation is ' + result)
                .ariaLabel('Alert Dialog Demo')
                .ok('Okay')
                .targetEvent(ev)
            );
        };

        $scope.onNetwork = function(ev) {
            var confirm = $mdDialog.confirm()
                .title('Are you sure you want to pairing a device over IP?')
                .textContent('')
                .targetEvent(ev)
                .ok('Okay')
                .cancel('Cancel');

            $mdDialog.show(confirm).then(function() {
                $scope.showLoadingSpinner = true;
                var data = {
                    nodeId: $scope.pairing.nodeId,
                    pinCode: $scope.pairing.pinCode,
                    type: $scope.pairing.type_onnetwork,
                };
                var httpRequest = $http({
                    method: 'POST',
                    url: 'pairing',
                    data: data,
                });

                httpRequest.then(function successCallback(response) {
                    $scope.showLoadingSpinner = false;
                    if (response.data.result == "successful") {
                        $scope.showAlert(event, 'Onnetwork', 'success');
                    } else {
                        $scope.showAlert(event, 'Onnetwork', 'failed');
                    }
                    ev.target.disabled = false;
                });
            });
        };

        $scope.bleWifi = function(ev) {
            var confirm = $mdDialog.confirm()
                .title('Are you sure you want to pairing a device over WIFI?')
                .textContent('')
                .targetEvent(ev)
                .ok('Okay')
                .cancel('Cancel');

            $mdDialog.show(confirm).then(function() {
                $scope.showLoadingSpinner = true;
                var data = {
                    nodeId: $scope.pairing.nodeId,
                    ssId: $scope.pairing.ssId,
                    password: $scope.pairing.password,
                    pinCode: $scope.pairing.pinCode,
                    discriminator: $scope.pairing.discriminator,
                    type: $scope.pairing.type_ble_wifi,
                };
                var httpRequest = $http({
                    method: 'POST',
                    url: 'pairing',
                    data: data,
                });

                httpRequest.then(function successCallback(response) {
                    $scope.showLoadingSpinner = false;
                    if (response.data.result == "successful") {
                        $scope.showAlert(event, 'ble-wifi', 'success');
                    } else {
                        $scope.showAlert(event, 'ble-wifi', 'failed');
                    }
                    ev.target.disabled = false;
                });
            });
        };

        $scope.getDataset = function(ev) {
            var confirm = $mdDialog.confirm()
                .title('This button can automatically trigger the ot-ctl command and obtain the dataset. But before using it, please confirm that the thread network has been successfully formed.')
                .textContent('')
                .targetEvent(ev)
                .ok('Okay')
                .cancel('Cancel');

            $mdDialog.show(confirm).then(function() {
                $scope.showLoadingSpinner = true;
                var httpRequest = $http({
                    method: 'GET',
                    url: 'get_dataset',
                });

                httpRequest.then(function successCallback(response) {
                    $scope.showLoadingSpinner = false;
                    console.log(response);
                    if (response.data.result == "successful") {
                        $scope.showAlert(event, 'get-dataset', 'success');
                        $scope.pairing.dataset = response.data.dataset;
                    } else {
                        $scope.showAlert(event, 'get-dataset', 'failed');
                    }
                    ev.target.disabled = false;
                });
            });
        };

        $scope.bleThread = function(ev) {
            var confirm = $mdDialog.confirm()
                .title('Are you sure you want to pairing a device over Thread?')
                .textContent('')
                .targetEvent(ev)
                .ok('Okay')
                .cancel('Cancel');

            $mdDialog.show(confirm).then(function() {
                $scope.showLoadingSpinner = true;
                var data = {
                    nodeId: $scope.pairing.nodeId,
                    dataset: $scope.pairing.dataset,
                    pinCode: $scope.pairing.pinCode,
                    discriminator: $scope.pairing.discriminator,
                    type: $scope.pairing.type_ble_thread,
                };
                var httpRequest = $http({
                    method: 'POST',
                    url: 'pairing',
                    data: data,
                });

                httpRequest.then(function successCallback(response) {
                    $scope.showLoadingSpinner = false;
                    if (response.data.result == "successful") {
                        $scope.showAlert(event, 'ble-thread', 'success');
                    } else {
                        $scope.showAlert(event, 'ble-thread', 'failed');
                    }
                    ev.target.disabled = false;
                });
            });
        };

        $scope.turnOnLight = function(ev) {
            $scope.showLoadingSpinner = true;
            var data = {
                nodeId: $scope.onoff.nodeId,
                endPointId: $scope.onoff.endPointId,
                type: $scope.onoff.type_on,
            };
            var httpRequest = $http({
                method: 'POST',
                url: 'onoff',
                data: data,
            });

            httpRequest.then(function successCallback(response) {
                $scope.showLoadingSpinner = false;
                if (response.data.result == "successful") {
                    $scope.showAlert(event, 'turn on', 'success');
                } else {
                    $scope.showAlert(event, 'turn on', 'failed');
                }
                ev.target.disabled = false;
            });
        };

        $scope.toggleLight = function(ev) {
            $scope.showLoadingSpinner = true;
            var data = {
                nodeId: $scope.onoff.nodeId,
                endPointId: $scope.onoff.endPointId,
                type: $scope.onoff.type_toggle,
            };
            var httpRequest = $http({
                method: 'POST',
                url: 'onoff',
                data: data,
            });

            httpRequest.then(function successCallback(response) {
                $scope.showLoadingSpinner = false;
                if (response.data.result == "successful") {
                    $scope.showAlert(event, 'toggle', 'success');
                } else {
                    $scope.showAlert(event, 'toggle', 'failed');
                }
                ev.target.disabled = false;
            });
        };

        $scope.turnOffLight = function(ev) {
            $scope.showLoadingSpinner = true;
            var data = {
                nodeId: $scope.onoff.nodeId,
                endPointId: $scope.onoff.endPointId,
                type: $scope.onoff.type_off,
            };
            var httpRequest = $http({
                method: 'POST',
                url: 'onoff',
                data: data,
            });

            httpRequest.then(function successCallback(response) {
                $scope.showLoadingSpinner = false;
                if (response.data.result == "successful") {
                    $scope.showAlert(event, 'turn off', 'success');
                } else {
                    $scope.showAlert(event, 'turn off', 'failed');
                }
                ev.target.disabled = false;
            });
        };

        $scope.read = function(ev) {
            $scope.showLoadingSpinner = true;
            var data = {
                attr: $scope.onoff.attr,
                nodeId: $scope.onoff.nodeId,
                endPointId: $scope.onoff.endPointId,
                type: $scope.onoff.type_read,
            };
            var httpReadReq = $http({
                method: 'POST',
                url: 'onoff',
                data: data,
            });

            httpReadReq.then(function successCallback(response) {
                console.log(response);
                if (response.data.result == "successful") {
                    var httpRequest = $http({
                    method: 'POST',
                    url: 'get_report',
                    data: data,
                    });
                    httpRequest.then(function successCallback(response) {
                        //$scope.reports = [];
                        console.log(response);
                        $scope.showLoadingSpinner = false;
                        if (response.data.result == "successful") {
                            $scope.showAlert(event, 'read operation is success, and get report', 'success');
                            var reportsStr = response.data.report;
                            $scope.reports.reverse();
                            $scope.reports.push({
                                report: reportsStr,
                                icon: 'res/img/icon-info.png',
                            });
                            $scope.reports.reverse();
                        } else {
                            $scope.showAlert(event, 'read operation is success, but get report', 'failed');
                        }
                    });
                } else {
                    $scope.showLoadingSpinner = false;
                    $scope.showAlert(event, 'read', 'failed');
                }
            });
        };

        $scope.wsServerPort = '9002';
        $scope.triggerSubscribe = function(ev){
            var ws = new WebSocket("ws://" + window.location.hostname + ":" + $scope.wsServerPort);
            console.log("New WebSocket connected.");
            $scope.showLoadingSpinner = true;
            ws.onopen = function(ev) {
                var msg = $scope.subscribe.subscribe_cluster + ' ' + $scope.subscribe.minInterval + ' ' + $scope.subscribe.maxInterval + ' ' + $scope.subscribe.nodeId + ' ' + $scope.subscribe.endPointId;
                ws.send(msg);
                $scope.showLoadingSpinner = false;
            };
            ws.onerror = function(ev) {
                console.log("WebSocket error!");
            };
            ws.onmessage = function(ev) {
                $scope.$apply(function() {
                    var statusStr = ev.data;
                    $scope.subscribeStatus.reverse();
                    $scope.subscribeStatus.push({
                        status: statusStr,
                        icon: 'res/img/icon-info.png',
                    });
                    $scope.subscribeStatus.reverse();
                })
            }
            ws.onclose = function(ev) {
                console.log("WebSocket connection closed.");
            };
        };

        $scope.restServerPort = '8889';
        $scope.ipAddr = window.location.hostname + ':' + $scope.restServerPort;
    };
})();
